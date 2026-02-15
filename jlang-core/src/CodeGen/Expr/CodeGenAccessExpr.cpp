#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

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
    const llvm::DataLayout &dataLayout = m_Module->getDataLayout();

    if (node.allocType.isArrayType())
    {
        // Array allocation: elementSize * arraySize
        TypeRef elemRef;
        elemRef.name = node.allocType.name;
        llvm::Type *elemType = MapType(elemRef);
        uint64_t elemSize = dataLayout.getTypeAllocSize(elemType);
        allocSize = elemSize * node.allocType.arraySize;
    }
    else
    {
        // Check if we're allocating a struct
        StructInfo *structInfo = m_symbols.LookupStruct(node.allocType.name);
        if (structInfo)
        {
            llvm::StructType *structType = structInfo->llvmType;
            allocSize = dataLayout.getTypeAllocSize(structType);
        }
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
    std::string structTypeName = DetermineStructTypeName(node.object.get());

    if (structTypeName.empty())
    {
        JLANG_ERROR("Cannot determine struct type for member access");
        return;
    }

    // Check for nullable
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.object.get()))
    {
        VariableInfo *typeInfo = m_symbols.LookupVariable(varExpr->name);
        if (typeInfo && typeInfo->type.isNullable)
        {
            JLANG_ERROR(STR("Cannot access member '%s' on nullable type '%s*?'. Check for null first.",
                            node.memberName.c_str(), typeInfo->type.name.c_str()));
            return;
        }
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

void CodeGenerator::VisitMethodCallExpr(MethodCallExpr &node)
{
    // Determine the type of the object
    std::string typeName = DetermineStructTypeName(node.object.get());

    if (typeName.empty())
    {
        JLANG_ERROR("Cannot determine type for method call");
        return;
    }

    // Check if the type is an interface (virtual dispatch)
    InterfaceInfo *ifaceInfo = m_symbols.LookupInterface(typeName);
    if (ifaceInfo)
    {
        // Interface virtual dispatch: load from fat pointer
        // The variable is an alloca of fat_ptr type
        VariableInfo *varInfo = nullptr;
        if (auto *varExpr = dynamic_cast<VarExpr *>(node.object.get()))
        {
            varInfo = m_symbols.LookupVariable(varExpr->name);
        }

        if (!varInfo)
        {
            JLANG_ERROR("Cannot resolve interface variable for method call");
            return;
        }

        llvm::AllocaInst *fatPtrAlloca = llvm::dyn_cast<llvm::AllocaInst>(varInfo->value);
        if (!fatPtrAlloca)
        {
            JLANG_ERROR("Interface variable is not an alloca");
            return;
        }

        // Find the method in the interface
        InterfaceMethodInfo *methodInfo = nullptr;
        for (auto &m : ifaceInfo->methods)
        {
            if (m.name == node.methodName)
            {
                methodInfo = &m;
                break;
            }
        }

        if (!methodInfo)
        {
            JLANG_ERROR(
                STR("Method '%s' not found in interface '%s'", node.methodName.c_str(), typeName.c_str()));
            return;
        }

        // Load data pointer (index 0)
        llvm::Value *dataField =
            m_IRBuilder.CreateStructGEP(ifaceInfo->fatPtrType, fatPtrAlloca, 0, "data_gep");
        llvm::Type *i8Ptr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context));
        llvm::Value *dataPtr = m_IRBuilder.CreateLoad(i8Ptr, dataField, "data_ptr");

        // Load vtable pointer (index 1)
        llvm::Value *vtableField =
            m_IRBuilder.CreateStructGEP(ifaceInfo->fatPtrType, fatPtrAlloca, 1, "vtable_gep");
        llvm::Type *vtablePtrType = llvm::PointerType::getUnqual(ifaceInfo->vtableType);
        llvm::Value *vtablePtr = m_IRBuilder.CreateLoad(vtablePtrType, vtableField, "vtable_ptr");

        // GEP into vtable for method slot
        llvm::Value *funcSlotPtr = m_IRBuilder.CreateStructGEP(ifaceInfo->vtableType, vtablePtr,
                                                               methodInfo->vtableIndex, "func_slot");

        // Load function pointer
        llvm::Type *funcPtrType = llvm::PointerType::getUnqual(methodInfo->llvmFuncType);
        llvm::Value *funcPtr = m_IRBuilder.CreateLoad(funcPtrType, funcSlotPtr, "func_ptr");

        // Build args: data_ptr (as i8*), then user args
        std::vector<llvm::Value *> args;
        args.push_back(dataPtr);

        for (auto &arg : node.arguments)
        {
            arg->Accept(*this);
            if (!m_LastValue)
            {
                JLANG_ERROR("Invalid argument in method call");
                return;
            }
            args.push_back(m_LastValue);
        }

        // Indirect call
        if (methodInfo->returnType.name == "void")
        {
            m_IRBuilder.CreateCall(methodInfo->llvmFuncType, funcPtr, args);
            m_LastValue = nullptr;
        }
        else
        {
            m_LastValue = m_IRBuilder.CreateCall(methodInfo->llvmFuncType, funcPtr, args, "vcall");
        }
        return;
    }

    // Concrete struct: direct call to StructName_methodName
    StructInfo *structInfo = m_symbols.LookupStruct(typeName);
    if (!structInfo)
    {
        JLANG_ERROR(STR("Unknown type '%s' for method call", typeName.c_str()));
        return;
    }

    std::string mangledName = typeName + "_" + node.methodName;
    llvm::Function *func = m_Module->getFunction(mangledName);
    if (!func)
    {
        JLANG_ERROR(
            STR("Method '%s' not found (looked for '%s')", node.methodName.c_str(), mangledName.c_str()));
        return;
    }

    // Build args: object pointer first (self), then user args
    node.object->Accept(*this);
    llvm::Value *objPtr = m_LastValue;
    if (!objPtr)
    {
        JLANG_ERROR("Invalid object in method call");
        return;
    }

    std::vector<llvm::Value *> args;
    args.push_back(objPtr);

    for (auto &arg : node.arguments)
    {
        arg->Accept(*this);
        if (!m_LastValue)
        {
            JLANG_ERROR("Invalid argument in method call");
            return;
        }
        args.push_back(m_LastValue);
    }

    if (func->getReturnType()->isVoidTy())
    {
        m_IRBuilder.CreateCall(func, args);
        m_LastValue = nullptr;
    }
    else
    {
        m_LastValue = m_IRBuilder.CreateCall(func, args, mangledName + "_call");
    }
}

void CodeGenerator::VisitMemberAssignExpr(MemberAssignExpr &node)
{
    // Get the object pointer
    node.object->Accept(*this);
    llvm::Value *objectPtr = m_LastValue;

    if (!objectPtr)
    {
        JLANG_ERROR("Invalid object in member assignment");
        return;
    }

    // Determine struct type
    std::string structTypeName = DetermineStructTypeName(node.object.get());
    if (structTypeName.empty())
    {
        JLANG_ERROR("Cannot determine struct type for member assignment");
        return;
    }

    StructInfo *structInfoPtr = m_symbols.LookupStruct(structTypeName);
    if (!structInfoPtr)
    {
        JLANG_ERROR(STR("Unknown struct type: %s", structTypeName.c_str()));
        return;
    }

    auto fieldIt = structInfoPtr->fields.find(node.memberName);
    if (fieldIt == structInfoPtr->fields.end())
    {
        JLANG_ERROR(
            STR("Unknown field '%s' in struct '%s'", node.memberName.c_str(), structTypeName.c_str()));
        return;
    }

    const FieldInfo &fieldInfo = fieldIt->second;

    if (!fieldInfo.isPublic)
    {
        JLANG_ERROR(STR("Cannot assign to private field '%s' in struct '%s'", node.memberName.c_str(),
                        structTypeName.c_str()));
        return;
    }

    // GEP to the field
    llvm::Value *fieldPtr = m_IRBuilder.CreateStructGEP(structInfoPtr->llvmType, objectPtr, fieldInfo.index,
                                                        node.memberName + "_ptr");

    // Evaluate the value
    node.value->Accept(*this);
    llvm::Value *valueToStore = m_LastValue;

    if (!valueToStore)
    {
        JLANG_ERROR("Invalid value in member assignment");
        return;
    }

    m_IRBuilder.CreateStore(valueToStore, fieldPtr);
    m_LastValue = valueToStore;
}

} // namespace jlang
