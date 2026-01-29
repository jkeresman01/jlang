#include "CodeGen.h"

#include "../Common/Logger.h"

namespace jlang
{

void CodeGenerator::VisitCallExpr(CallExpr &node)
{
    llvm::Function *callee = m_Module->getFunction(node.callee);

    if (!callee)
    {
        JLANG_ERROR(STR("Unknown function: %s", node.callee.c_str()));
    }

    std::vector<llvm::Value *> args;

    for (auto &arg : node.arguments)
    {
        arg->Accept(*this);

        if (!m_LastValue)
        {
            JLANG_ERROR(STR("Invalid argument in call to %s", node.callee.c_str()));
        }
        args.push_back(m_LastValue);
    }

    m_LastValue = m_IRBuilder.CreateCall(callee, args, node.callee + "_call");
}

void CodeGenerator::VisitBinaryExpr(BinaryExpr &node)
{
    // Handle short-circuit operators separately - they must not evaluate RHS eagerly
    if (node.op == "&&")
    {
        // Short-circuit AND: if left is false, result is false; otherwise evaluate right
        node.left->Accept(*this);
        llvm::Value *leftVal = m_LastValue;

        if (!leftVal)
        {
            JLANG_ERROR("Invalid left operand in && expression");
            return;
        }

        llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();

        llvm::BasicBlock *rhsBlock = llvm::BasicBlock::Create(m_Context, "and.rhs", parentFunction);
        llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(m_Context, "and.merge");

        // Convert left to i1 if needed
        llvm::Value *leftBool = leftVal;
        if (!leftVal->getType()->isIntegerTy(1))
        {
            leftBool =
                m_IRBuilder.CreateICmpNE(leftVal, llvm::ConstantInt::get(leftVal->getType(), 0), "tobool");
        }

        llvm::BasicBlock *entryBlock = m_IRBuilder.GetInsertBlock();
        m_IRBuilder.CreateCondBr(leftBool, rhsBlock, mergeBlock);

        // RHS block - only now evaluate the right operand
        m_IRBuilder.SetInsertPoint(rhsBlock);
        node.right->Accept(*this);
        llvm::Value *rightVal = m_LastValue;

        if (!rightVal)
        {
            JLANG_ERROR("Invalid right operand in && expression");
            return;
        }

        llvm::Value *rightBool = rightVal;
        if (!rightVal->getType()->isIntegerTy(1))
        {
            rightBool =
                m_IRBuilder.CreateICmpNE(rightVal, llvm::ConstantInt::get(rightVal->getType(), 0), "tobool");
        }
        llvm::BasicBlock *rhsEndBlock = m_IRBuilder.GetInsertBlock();
        m_IRBuilder.CreateBr(mergeBlock);

        // Merge block
        mergeBlock->insertInto(parentFunction);
        m_IRBuilder.SetInsertPoint(mergeBlock);

        llvm::PHINode *phi = m_IRBuilder.CreatePHI(llvm::Type::getInt1Ty(m_Context), 2, "and.result");
        phi->addIncoming(llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 0), entryBlock);
        phi->addIncoming(rightBool, rhsEndBlock);

        m_LastValue = phi;
        return;
    }

    if (node.op == "||")
    {
        // Short-circuit OR: if left is true, result is true; otherwise evaluate right
        node.left->Accept(*this);
        llvm::Value *leftVal = m_LastValue;

        if (!leftVal)
        {
            JLANG_ERROR("Invalid left operand in || expression");
            return;
        }

        llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();

        llvm::BasicBlock *rhsBlock = llvm::BasicBlock::Create(m_Context, "or.rhs", parentFunction);
        llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(m_Context, "or.merge");

        // Convert left to i1 if needed
        llvm::Value *leftBool = leftVal;
        if (!leftVal->getType()->isIntegerTy(1))
        {
            leftBool =
                m_IRBuilder.CreateICmpNE(leftVal, llvm::ConstantInt::get(leftVal->getType(), 0), "tobool");
        }

        llvm::BasicBlock *entryBlock = m_IRBuilder.GetInsertBlock();
        m_IRBuilder.CreateCondBr(leftBool, mergeBlock, rhsBlock);

        // RHS block - only now evaluate the right operand
        m_IRBuilder.SetInsertPoint(rhsBlock);
        node.right->Accept(*this);
        llvm::Value *rightVal = m_LastValue;

        if (!rightVal)
        {
            JLANG_ERROR("Invalid right operand in || expression");
            return;
        }

        llvm::Value *rightBool = rightVal;
        if (!rightVal->getType()->isIntegerTy(1))
        {
            rightBool =
                m_IRBuilder.CreateICmpNE(rightVal, llvm::ConstantInt::get(rightVal->getType(), 0), "tobool");
        }
        llvm::BasicBlock *rhsEndBlock = m_IRBuilder.GetInsertBlock();
        m_IRBuilder.CreateBr(mergeBlock);

        // Merge block
        mergeBlock->insertInto(parentFunction);
        m_IRBuilder.SetInsertPoint(mergeBlock);

        llvm::PHINode *phi = m_IRBuilder.CreatePHI(llvm::Type::getInt1Ty(m_Context), 2, "or.result");
        phi->addIncoming(llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 1), entryBlock);
        phi->addIncoming(rightBool, rhsEndBlock);

        m_LastValue = phi;
        return;
    }

    // All other binary operators evaluate both sides
    node.left->Accept(*this);
    llvm::Value *leftVal = m_LastValue;

    node.right->Accept(*this);
    llvm::Value *rightVal = m_LastValue;

    if (!leftVal || !rightVal)
    {
        JLANG_ERROR("Invalid operands in binary expression");
        return;
    }

    if (node.op == "==")
    {
        if (leftVal->getType()->isPointerTy() && rightVal->getType()->isPointerTy())
        {
            m_LastValue = m_IRBuilder.CreateICmpEQ(leftVal, rightVal, "ptreq");
        }
        else if (leftVal->getType()->isIntegerTy() && rightVal->getType()->isIntegerTy())
        {
            m_LastValue = m_IRBuilder.CreateICmpEQ(leftVal, rightVal, "eq");
        }
        else
        {
            JLANG_ERROR("Unsupported types for == comparison");
        }
    }
    else if (node.op == "!=")
    {
        if (leftVal->getType()->isPointerTy() && rightVal->getType()->isPointerTy())
        {
            m_LastValue = m_IRBuilder.CreateICmpNE(leftVal, rightVal, "ptrne");
        }
        else if (leftVal->getType()->isIntegerTy() && rightVal->getType()->isIntegerTy())
        {
            m_LastValue = m_IRBuilder.CreateICmpNE(leftVal, rightVal, "ne");
        }
        else
        {
            JLANG_ERROR("Unsupported types for != comparison");
        }
    }
    else if (node.op == "<")
    {
        m_LastValue = m_IRBuilder.CreateICmpSLT(leftVal, rightVal, "lt");
    }
    else if (node.op == "<=")
    {
        m_LastValue = m_IRBuilder.CreateICmpSLE(leftVal, rightVal, "le");
    }
    else if (node.op == ">")
    {
        m_LastValue = m_IRBuilder.CreateICmpSGT(leftVal, rightVal, "gt");
    }
    else if (node.op == ">=")
    {
        m_LastValue = m_IRBuilder.CreateICmpSGE(leftVal, rightVal, "ge");
    }
    else if (node.op == "+")
    {
        m_LastValue = m_IRBuilder.CreateAdd(leftVal, rightVal, "add");
    }
    else if (node.op == "-")
    {
        m_LastValue = m_IRBuilder.CreateSub(leftVal, rightVal, "sub");
    }
    else if (node.op == "*")
    {
        m_LastValue = m_IRBuilder.CreateMul(leftVal, rightVal, "mul");
    }
    else if (node.op == "/")
    {
        m_LastValue = m_IRBuilder.CreateSDiv(leftVal, rightVal, "div");
    }
    else if (node.op == "%")
    {
        m_LastValue = m_IRBuilder.CreateSRem(leftVal, rightVal, "mod");
    }
    else if (node.op == "&")
    {
        m_LastValue = m_IRBuilder.CreateAnd(leftVal, rightVal, "bitand");
    }
    else if (node.op == "|")
    {
        m_LastValue = m_IRBuilder.CreateOr(leftVal, rightVal, "bitor");
    }
    else if (node.op == "^")
    {
        m_LastValue = m_IRBuilder.CreateXor(leftVal, rightVal, "bitxor");
    }
    else if (node.op == "<<")
    {
        m_LastValue = m_IRBuilder.CreateShl(leftVal, rightVal, "shl");
    }
    else if (node.op == ">>")
    {
        m_LastValue = m_IRBuilder.CreateAShr(leftVal, rightVal, "shr");
    }
    else if (node.op == "and")
    {
        // Non-short-circuit AND: both operands are always evaluated
        llvm::Value *leftBool = leftVal;
        if (!leftVal->getType()->isIntegerTy(1))
        {
            leftBool =
                m_IRBuilder.CreateICmpNE(leftVal, llvm::ConstantInt::get(leftVal->getType(), 0), "tobool");
        }

        llvm::Value *rightBool = rightVal;
        if (!rightVal->getType()->isIntegerTy(1))
        {
            rightBool =
                m_IRBuilder.CreateICmpNE(rightVal, llvm::ConstantInt::get(rightVal->getType(), 0), "tobool");
        }

        m_LastValue = m_IRBuilder.CreateAnd(leftBool, rightBool, "and.result");
    }
    else if (node.op == "or")
    {
        // Non-short-circuit OR: both operands are always evaluated
        llvm::Value *leftBool = leftVal;
        if (!leftVal->getType()->isIntegerTy(1))
        {
            leftBool =
                m_IRBuilder.CreateICmpNE(leftVal, llvm::ConstantInt::get(leftVal->getType(), 0), "tobool");
        }

        llvm::Value *rightBool = rightVal;
        if (!rightVal->getType()->isIntegerTy(1))
        {
            rightBool =
                m_IRBuilder.CreateICmpNE(rightVal, llvm::ConstantInt::get(rightVal->getType(), 0), "tobool");
        }

        m_LastValue = m_IRBuilder.CreateOr(leftBool, rightBool, "or.result");
    }
    else
    {
        JLANG_ERROR(STR("Unknown binary operator: %s", node.op.c_str()));
    }
}

void CodeGenerator::VisitUnaryExpr(UnaryExpr &node)
{
    node.operand->Accept(*this);
    llvm::Value *operandVal = m_LastValue;

    if (!operandVal)
    {
        JLANG_ERROR("Invalid operand in unary expression");
        return;
    }

    if (node.op == "!")
    {
        // Logical NOT
        llvm::Value *boolVal = operandVal;
        if (!operandVal->getType()->isIntegerTy(1))
        {
            boolVal = m_IRBuilder.CreateICmpNE(operandVal, llvm::ConstantInt::get(operandVal->getType(), 0),
                                               "tobool");
        }
        m_LastValue = m_IRBuilder.CreateXor(
            boolVal, llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 1), "not");
    }
    else if (node.op == "~")
    {
        // Bitwise NOT (flip all bits)
        m_LastValue = m_IRBuilder.CreateNot(operandVal, "bitnot");
    }
    else
    {
        JLANG_ERROR(STR("Unknown unary operator: %s", node.op.c_str()));
    }
}

void CodeGenerator::VisitLiteralExpr(LiteralExpr &node)
{
    if (node.value == "NULL" || node.value == "null" || node.value == "nullptr")
    {
        m_LastValue =
            llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context)));
    }
    else if (node.value == "true")
    {
        m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 1);
    }
    else if (node.value == "false")
    {
        m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 0);
    }
    else if (node.value.front() == '"' && node.value.back() == '"')
    {
        // String literal
        std::string strValue = node.value.substr(1, node.value.size() - 2);
        m_LastValue = m_IRBuilder.CreateGlobalStringPtr(strValue, "str");
    }
    else if (node.value.front() == '\'' && node.value.back() == '\'')
    {
        // Character literal - extract the character and create an i8 constant
        char charValue = node.value[1];
        m_LastValue =
            llvm::ConstantInt::get(llvm::Type::getInt8Ty(m_Context), static_cast<uint8_t>(charValue));
    }
    else if (node.value.find('.') != std::string::npos)
    {
        // Float literal
        try
        {
            double floatValue = std::stod(node.value);
            // Default to f64 (double) for now
            m_LastValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(m_Context), floatValue);
        }
        catch (...)
        {
            JLANG_ERROR(STR("Invalid float literal: %s", node.value.c_str()));
        }
    }
    else
    {
        // Integer literal
        try
        {
            int64_t intValue = std::stoll(node.value);
            m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), intValue);
        }
        catch (...)
        {
            JLANG_ERROR(STR("Unknown literal: %s", node.value.c_str()));
        }
    }
}

void CodeGenerator::VisitVarExpr(VarExpr &node)
{
    VariableInfo *varInfo = m_symbols.LookupVariable(node.name);

    if (!varInfo)
    {
        JLANG_ERROR(STR("Undefined variable: %s", node.name.c_str()));
        return;
    }

    // Mark variable as used
    varInfo->used = true;

    llvm::Value *storedValue = varInfo->value;

    if (llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(storedValue))
    {
        m_LastValue = m_IRBuilder.CreateLoad(alloca->getAllocatedType(), alloca, node.name);
    }
    else
    {
        m_LastValue = storedValue;
    }
}

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

void CodeGenerator::VisitPrefixExpr(PrefixExpr &node)
{
    // Prefix ++/--: increment/decrement and return the NEW value
    auto *varExpr = dynamic_cast<VarExpr *>(node.operand.get());
    if (!varExpr)
    {
        JLANG_ERROR("Prefix increment/decrement requires a variable operand");
        return;
    }

    VariableInfo *varInfo = m_symbols.LookupVariable(varExpr->name);
    if (!varInfo)
    {
        JLANG_ERROR(STR("Undefined variable: %s", varExpr->name.c_str()));
        return;
    }

    if (!varInfo->isMutable)
    {
        JLANG_ERROR(
            STR("Cannot modify immutable variable '%s' (declared with 'val')", varExpr->name.c_str()));
        return;
    }

    // Mark variable as used
    varInfo->used = true;

    llvm::Value *varPtr = varInfo->value;
    llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(varPtr);
    if (!alloca)
    {
        JLANG_ERROR("Cannot increment/decrement non-variable");
        return;
    }

    // Load current value
    llvm::Value *currentVal = m_IRBuilder.CreateLoad(alloca->getAllocatedType(), alloca, "load");

    // Add or subtract 1
    llvm::Value *one = llvm::ConstantInt::get(currentVal->getType(), 1);
    llvm::Value *newVal;
    if (node.op == "++")
    {
        newVal = m_IRBuilder.CreateAdd(currentVal, one, "inc");
    }
    else
    {
        newVal = m_IRBuilder.CreateSub(currentVal, one, "dec");
    }

    // Store new value back
    m_IRBuilder.CreateStore(newVal, alloca);

    // Return the NEW value
    m_LastValue = newVal;
}

void CodeGenerator::VisitPostfixExpr(PostfixExpr &node)
{
    // Postfix ++/--: increment/decrement and return the ORIGINAL value
    auto *varExpr = dynamic_cast<VarExpr *>(node.operand.get());
    if (!varExpr)
    {
        JLANG_ERROR("Postfix increment/decrement requires a variable operand");
        return;
    }

    VariableInfo *varInfo = m_symbols.LookupVariable(varExpr->name);
    if (!varInfo)
    {
        JLANG_ERROR(STR("Undefined variable: %s", varExpr->name.c_str()));
        return;
    }

    if (!varInfo->isMutable)
    {
        JLANG_ERROR(
            STR("Cannot modify immutable variable '%s' (declared with 'val')", varExpr->name.c_str()));
        return;
    }

    // Mark variable as used
    varInfo->used = true;

    llvm::Value *varPtr = varInfo->value;
    llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(varPtr);
    if (!alloca)
    {
        JLANG_ERROR("Cannot increment/decrement non-variable");
        return;
    }

    // Load current value
    llvm::Value *currentVal = m_IRBuilder.CreateLoad(alloca->getAllocatedType(), alloca, "load");

    // Add or subtract 1
    llvm::Value *one = llvm::ConstantInt::get(currentVal->getType(), 1);
    llvm::Value *newVal;
    if (node.op == "++")
    {
        newVal = m_IRBuilder.CreateAdd(currentVal, one, "inc");
    }
    else
    {
        newVal = m_IRBuilder.CreateSub(currentVal, one, "dec");
    }

    // Store new value back
    m_IRBuilder.CreateStore(newVal, alloca);

    // Return the ORIGINAL value
    m_LastValue = currentVal;
}

} // namespace jlang
