#include "../CodeGen.h"

#include "../../Common/Logger.h"

namespace jlang
{

void CodeGenerator::VisitCastExpr(CastExpr &node)
{
    node.expr->Accept(*this);
    llvm::Value *valueToCast = m_LastValue;

    if (!valueToCast)
    {
        JLANG_ERROR("Invalid expression in cast");
    }

    llvm::Type *targetLLVMType = MapType(node.targetType);

    if (!targetLLVMType)
    {
        JLANG_ERROR("Unknown target type in cast");
    }

    bool isPointerToPointerCast = valueToCast->getType()->isPointerTy() && targetLLVMType->isPointerTy();

    if (isPointerToPointerCast)
    {
        m_LastValue = m_IRBuilder.CreateBitCast(valueToCast, targetLLVMType, "ptrcast");
    }
    else
    {
        JLANG_ERROR("Unsupported cast");
    }
}

void CodeGenerator::VisitAllocExpr(AllocExpr &node)
{
    // Get malloc function
    llvm::Function *mallocFunc = m_Module->getFunction("malloc");
    if (!mallocFunc)
    {
        JLANG_ERROR("malloc not declared");
        return;
    }

    // Calculate size based on the type being allocated
    uint64_t allocSize = 8; // Default size

    // Check if we're allocating a struct
    StructInfo *structInfo = m_symbols.LookupStruct(node.allocType.name);
    if (structInfo)
    {
        // Get the size of the struct from LLVM's data layout
        llvm::StructType *structType = structInfo->llvmType;
        const llvm::DataLayout &dataLayout = m_Module->getDataLayout();
        allocSize = dataLayout.getTypeAllocSize(structType);
    }

    llvm::Value *size = llvm::ConstantInt::get(llvm::Type::getInt64Ty(m_Context), allocSize);

    // Call malloc
    llvm::Value *allocated = m_IRBuilder.CreateCall(mallocFunc, {size}, "alloc");

    // Cast to the appropriate pointer type
    llvm::Type *targetType = MapType(node.allocType);
    if (allocated->getType() != targetType)
    {
        allocated = m_IRBuilder.CreateBitCast(allocated, targetType, "alloc_cast");
    }

    m_LastValue = allocated;
}

void CodeGenerator::VisitAssignExpr(AssignExpr &node)
{
    // Evaluate the right-hand side
    node.value->Accept(*this);
    llvm::Value *valueToStore = m_LastValue;

    if (!valueToStore)
    {
        JLANG_ERROR("Invalid value in assignment");
        return;
    }

    // Find the variable
    VariableInfo *varInfo = m_symbols.LookupVariable(node.name);
    if (!varInfo)
    {
        JLANG_ERROR(STR("Undefined variable in assignment: %s", node.name.c_str()));
        return;
    }

    if (!varInfo->isMutable)
    {
        JLANG_ERROR(STR("Cannot assign to immutable variable '%s' (declared with 'val')", node.name.c_str()));
        return;
    }

    // Null safety: cannot assign null to non-nullable pointer
    if (varInfo->type.isPointer && !varInfo->type.isNullable &&
        llvm::isa<llvm::ConstantPointerNull>(valueToStore))
    {
        JLANG_ERROR(STR("Cannot assign null to non-nullable variable '%s'.", node.name.c_str()));
        return;
    }

    llvm::Value *targetVar = varInfo->value;

    if (llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(targetVar))
    {
        m_IRBuilder.CreateStore(valueToStore, alloca);
        m_LastValue = valueToStore;
    }
    else
    {
        JLANG_ERROR("Cannot assign to non-variable");
    }
}

void CodeGenerator::VisitMemberAccessExpr(MemberAccessExpr &node)
{
    // Get the object (should be a pointer to a struct)
    node.object->Accept(*this);
    llvm::Value *objectPtr = m_LastValue;

    if (!objectPtr)
    {
        JLANG_ERROR("Invalid object in member access");
        return;
    }

    // Determine the struct type from the object
    std::string structTypeName;

    // If the object is a VarExpr, look up its type
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.object.get()))
    {
        VariableInfo *typeInfo = m_symbols.LookupVariable(varExpr->name);
        if (typeInfo)
        {
            // Null safety: cannot access member on nullable pointer
            if (typeInfo->type.isNullable)
            {
                JLANG_ERROR(STR("Cannot access member '%s' on nullable type '%s*?'. Check for null first.",
                                node.memberName.c_str(), typeInfo->type.name.c_str()));
                return;
            }

            structTypeName = typeInfo->type.name;
        }
    }

    if (structTypeName.empty())
    {
        JLANG_ERROR("Cannot determine struct type for member access");
        return;
    }

    // Find the struct info
    StructInfo *structInfoPtr = m_symbols.LookupStruct(structTypeName);
    if (!structInfoPtr)
    {
        JLANG_ERROR(STR("Unknown struct type: %s", structTypeName.c_str()));
        return;
    }

    const StructInfo &structInfo = *structInfoPtr;

    // Find the field
    auto fieldIt = structInfo.fields.find(node.memberName);
    if (fieldIt == structInfo.fields.end())
    {
        JLANG_ERROR(
            STR("Unknown field '%s' in struct '%s'", node.memberName.c_str(), structTypeName.c_str()));
        return;
    }

    const FieldInfo &fieldInfo = fieldIt->second;

    // Check visibility - private fields (lowercase) can only be accessed from within the struct's methods
    if (!fieldInfo.isPublic)
    {
        JLANG_ERROR(STR("Cannot access private field '%s' in struct '%s'", node.memberName.c_str(),
                        structTypeName.c_str()));
        return;
    }

    // Generate GEP to access the field
    llvm::Value *fieldPtr = m_IRBuilder.CreateStructGEP(structInfo.llvmType, objectPtr, fieldInfo.index,
                                                        node.memberName + "_ptr");

    // Load the field value
    llvm::Type *fieldType = MapType(fieldInfo.type);
    m_LastValue = m_IRBuilder.CreateLoad(fieldType, fieldPtr, node.memberName);
}

} // namespace jlang
