#include "../CodeGen.h"

#include "../../Common/Logger.h"

#include <algorithm>

#include <llvm/IR/Verifier.h>

namespace jlang
{

void CodeGenerator::VisitFunctionDecl(FunctionDecl &node)
{
    // Clear tracking for new function scope
    m_symbols.EnterFunctionScope();

    std::vector<llvm::Type *> paramTypes;
    paramTypes.reserve(node.params.size());
    std::transform(node.params.begin(), node.params.end(), std::back_inserter(paramTypes),
                   [this](const Parameter &param) { return MapType(param.type); });

    llvm::FunctionType *funcType = llvm::FunctionType::get(MapType(node.returnType), paramTypes, false);

    llvm::Function *function =
        llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, node.name, m_Module.get());

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(m_Context, "entry", function);
    m_IRBuilder.SetInsertPoint(entry);

    unsigned paramIndex = 0;
    for (auto &arg : function->args())
    {
        const std::string &paramName = node.params[paramIndex].name;
        arg.setName(paramName);
        m_symbols.DefineVariable(paramName, VariableInfo{&arg, node.params[paramIndex].type, false});
        m_symbols.TrackFunctionLocal(paramName);
        ++paramIndex;
    }

    if (node.body)
    {
        node.body->Accept(*this);
    }

    if (node.returnType.name == "void")
    {
        m_IRBuilder.CreateRetVoid();
    }

    llvm::verifyFunction(*function);

    // Clean up function-local variables
    m_symbols.LeaveFunctionScope();
}

void CodeGenerator::VisitInterfaceDecl(InterfaceDecl &) {}

void CodeGenerator::VisitStructDecl(StructDecl &node)
{
    // Create LLVM struct type
    std::vector<llvm::Type *> fieldTypes;
    fieldTypes.reserve(node.fields.size());

    StructInfo structInfo;

    for (unsigned i = 0; i < node.fields.size(); ++i)
    {
        const auto &field = node.fields[i];
        llvm::Type *fieldType = MapType(field.type);
        fieldTypes.push_back(fieldType);

        structInfo.fields[field.name] = FieldInfo{i, field.type, field.isPublic};
    }

    llvm::StructType *structType = llvm::StructType::create(m_Context, fieldTypes, node.name);
    structInfo.llvmType = structType;

    m_symbols.DefineStruct(node.name, structInfo);
}

void CodeGenerator::VisitVariableDecl(VariableDecl &node)
{
    llvm::Type *varType = nullptr;
    TypeRef inferredType = node.varType;

    // Check if type inference is needed (empty type name)
    if (node.varType.name.empty())
    {
        if (!node.initializer)
        {
            JLANG_ERROR(STR("Type inference requires an initializer for variable: %s", node.name.c_str()));
            return;
        }

        // Evaluate initializer first to get its type
        node.initializer->Accept(*this);
        if (!m_LastValue)
        {
            JLANG_ERROR(STR("Invalid initializer for variable: %s", node.name.c_str()));
            return;
        }

        // Check if the initializer produced an array alloca (from array literal)
        if (llvm::AllocaInst *srcAlloca = llvm::dyn_cast<llvm::AllocaInst>(m_LastValue))
        {
            if (srcAlloca->getAllocatedType()->isArrayTy())
            {
                varType = srcAlloca->getAllocatedType();
                inferredType = InferTypeRef(varType);

                llvm::AllocaInst *alloca = m_IRBuilder.CreateAlloca(varType, nullptr, node.name);
                uint64_t typeSize = m_Module->getDataLayout().getTypeAllocSize(varType);
                m_IRBuilder.CreateMemCpy(alloca, llvm::MaybeAlign(), srcAlloca, llvm::MaybeAlign(), typeSize);

                m_symbols.DefineVariable(node.name,
                                         VariableInfo{alloca, inferredType, false, node.isMutable});
                m_symbols.TrackFunctionLocal(node.name);
                return;
            }
        }

        varType = m_LastValue->getType();
        inferredType = InferTypeRef(varType);

        // Create alloca and store the already-computed value
        llvm::AllocaInst *alloca = m_IRBuilder.CreateAlloca(varType, nullptr, node.name);
        m_IRBuilder.CreateStore(m_LastValue, alloca);

        // Track variable with inferred type
        m_symbols.DefineVariable(node.name, VariableInfo{alloca, inferredType, false, node.isMutable});
        m_symbols.TrackFunctionLocal(node.name);
        return;
    }

    // Explicit type - use existing logic
    varType = MapType(node.varType);

    // Null safety: non-nullable pointers must be initialized
    if (node.varType.isPointer && !node.varType.isNullable && !node.initializer)
    {
        JLANG_ERROR(STR("Non-nullable pointer '%s' must be initialized.", node.name.c_str()));
        return;
    }

    llvm::AllocaInst *alloca = m_IRBuilder.CreateAlloca(varType, nullptr, node.name);

    if (node.initializer)
    {
        // Check if initializer is an array literal being assigned to an array type
        if (node.varType.isArrayType() && node.initializer->type == NodeType::ArrayLiteralExpr)
        {
            // Visit the array literal - it creates its own alloca
            node.initializer->Accept(*this);
            if (m_LastValue)
            {
                // m_LastValue is an alloca for the array literal.
                // We need to memcpy from the literal's alloca to our variable's alloca.
                llvm::AllocaInst *srcAlloca = llvm::dyn_cast<llvm::AllocaInst>(m_LastValue);
                if (srcAlloca)
                {
                    uint64_t typeSize = m_Module->getDataLayout().getTypeAllocSize(varType);
                    m_IRBuilder.CreateMemCpy(alloca, llvm::MaybeAlign(), srcAlloca, llvm::MaybeAlign(),
                                             typeSize);
                }
            }
        }
        else
        {
            node.initializer->Accept(*this);
            if (m_LastValue)
            {
                // Null safety: cannot assign null to non-nullable pointer
                if (node.varType.isPointer && !node.varType.isNullable &&
                    llvm::isa<llvm::ConstantPointerNull>(m_LastValue))
                {
                    JLANG_ERROR(STR(
                        "Cannot assign null to non-nullable pointer type '%s*'. Use '%s*?' to allow null.",
                        node.varType.name.c_str(), node.varType.name.c_str()));
                    return;
                }

                if (m_LastValue->getType() != varType && varType->isPointerTy() &&
                    m_LastValue->getType()->isPointerTy())
                {
                    m_LastValue = m_IRBuilder.CreateBitCast(m_LastValue, varType, "cast");
                }
                m_IRBuilder.CreateStore(m_LastValue, alloca);
            }
        }
    }

    // Track variable with usage info (initially unused)
    m_symbols.DefineVariable(node.name, VariableInfo{alloca, node.varType, false, node.isMutable});
    m_symbols.TrackFunctionLocal(node.name);
}

} // namespace jlang
