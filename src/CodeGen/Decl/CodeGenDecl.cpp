#include "../CodeGen.h"

#include "../../Common/Logger.h"

#include <algorithm>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>

namespace jlang
{

void CodeGenerator::VisitFunctionDecl(FunctionDecl &node)
{
    // Clear tracking for new function scope
    m_symbols.EnterFunctionScope();

    // Name mangling: if first param is self: StructName*, use StructName_methodName
    std::string llvmName = node.name;
    if (!node.params.empty() && node.params[0].name == "self" && node.params[0].type.isPointer)
    {
        StructInfo *si = m_symbols.LookupStruct(node.params[0].type.name);
        if (si)
        {
            llvmName = node.params[0].type.name + "_" + node.name;
        }
    }

    // Reuse the function created in pass 2
    llvm::Function *function = m_Module->getFunction(llvmName);
    if (!function)
    {
        JLANG_ERROR(STR("Internal error: function '%s' not found in module", llvmName.c_str()));
        m_symbols.LeaveFunctionScope();
        return;
    }

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(m_Context, "entry", function);
    m_IRBuilder.SetInsertPoint(entry);

    unsigned paramIndex = 0;
    for (auto &arg : function->args())
    {
        const std::string &paramName = node.params[paramIndex].name;
        const TypeRef &paramType = node.params[paramIndex].type;
        arg.setName(paramName);

        // For interface/struct-value parameters, create an alloca so we can GEP into them
        InterfaceInfo *ifaceParam = m_symbols.LookupInterface(paramType.name);
        if (ifaceParam && !paramType.isPointer)
        {
            // Store the fat pointer arg into an alloca
            llvm::AllocaInst *alloca =
                m_IRBuilder.CreateAlloca(ifaceParam->fatPtrType, nullptr, paramName + "_addr");
            m_IRBuilder.CreateStore(&arg, alloca);
            m_symbols.DefineVariable(paramName, VariableInfo{alloca, paramType, false});
        }
        else
        {
            m_symbols.DefineVariable(paramName, VariableInfo{&arg, paramType, false});
        }
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

void CodeGenerator::VisitInterfaceDecl(InterfaceDecl &node)
{
    llvm::Type *i8Ptr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context));

    InterfaceInfo ifaceInfo;
    ifaceInfo.name = node.name;

    std::vector<llvm::Type *> vtableSlotTypes;

    for (unsigned idx = 0; idx < node.methods.size(); ++idx)
    {
        auto &method = node.methods[idx];

        // Build function type: retType (i8* self, params...)
        std::vector<llvm::Type *> funcParamTypes;
        funcParamTypes.push_back(i8Ptr); // self as i8*

        std::vector<TypeRef> paramTypeRefs;
        for (auto &param : method.params)
        {
            funcParamTypes.push_back(MapType(param.type));
            paramTypeRefs.push_back(param.type);
        }

        llvm::Type *retType = MapType(method.returnType);
        llvm::FunctionType *funcType = llvm::FunctionType::get(retType, funcParamTypes, false);

        // Vtable slot is a pointer to this function type
        vtableSlotTypes.push_back(llvm::PointerType::getUnqual(funcType));

        InterfaceMethodInfo methodInfo;
        methodInfo.name = method.name;
        methodInfo.paramTypes = paramTypeRefs;
        methodInfo.returnType = method.returnType;
        methodInfo.vtableIndex = idx;
        methodInfo.llvmFuncType = funcType;

        ifaceInfo.methods.push_back(methodInfo);
    }

    // Create vtable struct type
    llvm::StructType *vtableType =
        llvm::StructType::create(m_Context, vtableSlotTypes, node.name + "_vtable_t");
    ifaceInfo.vtableType = vtableType;

    // Create fat pointer type: { i8*, vtable_t* }
    llvm::StructType *fatPtrType = llvm::StructType::create(
        m_Context, {i8Ptr, llvm::PointerType::getUnqual(vtableType)}, node.name + "_fat_ptr");
    ifaceInfo.fatPtrType = fatPtrType;

    m_symbols.DefineInterface(node.name, ifaceInfo);
}

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
    structInfo.interfaceImplemented = node.interfaceImplemented;

    m_symbols.DefineStruct(node.name, structInfo);

    // Record struct-interface mapping (vtable global will be created later)
    if (!node.interfaceImplemented.empty())
    {
        StructInterfaceInfo siInfo;
        siInfo.interfaceName = node.interfaceImplemented;
        siInfo.vtableGlobal = nullptr; // filled in by GenerateVtables()
        m_symbols.DefineStructInterface(node.name, siInfo);
    }
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

    // Check if declared type is an interface (implicit struct*-to-interface conversion)
    InterfaceInfo *ifaceInfo = m_symbols.LookupInterface(node.varType.name);
    if (ifaceInfo && node.initializer)
    {
        // Evaluate the initializer (should be a struct pointer)
        node.initializer->Accept(*this);
        if (!m_LastValue)
        {
            JLANG_ERROR(STR("Invalid initializer for interface variable: %s", node.name.c_str()));
            return;
        }

        // Determine the struct type name from the initializer
        std::string structTypeName = DetermineStructTypeName(node.initializer.get());
        if (structTypeName.empty())
        {
            JLANG_ERROR("Cannot determine struct type for interface conversion");
            return;
        }

        StructInterfaceInfo *siInfo = m_symbols.LookupStructInterface(structTypeName);
        if (!siInfo || !siInfo->vtableGlobal)
        {
            JLANG_ERROR(STR("Struct '%s' does not implement interface '%s' (no vtable)",
                            structTypeName.c_str(), node.varType.name.c_str()));
            return;
        }

        // Create fat pointer: { bitcast(ptr, i8*), &vtable }
        llvm::Type *i8Ptr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context));
        llvm::Value *dataPtr = m_IRBuilder.CreateBitCast(m_LastValue, i8Ptr, "data_ptr");

        llvm::AllocaInst *alloca = m_IRBuilder.CreateAlloca(ifaceInfo->fatPtrType, nullptr, node.name);

        // Store data pointer at index 0
        llvm::Value *dataField = m_IRBuilder.CreateStructGEP(ifaceInfo->fatPtrType, alloca, 0, "fat_data");
        m_IRBuilder.CreateStore(dataPtr, dataField);

        // Store vtable pointer at index 1
        llvm::Value *vtableField =
            m_IRBuilder.CreateStructGEP(ifaceInfo->fatPtrType, alloca, 1, "fat_vtable");
        m_IRBuilder.CreateStore(siInfo->vtableGlobal, vtableField);

        m_symbols.DefineVariable(node.name, VariableInfo{alloca, node.varType, false, node.isMutable});
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
