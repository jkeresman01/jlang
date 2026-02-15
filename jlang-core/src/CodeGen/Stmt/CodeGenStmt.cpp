#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

namespace jlang
{

void CodeGenerator::VisitIfStatement(IfStatement &node)
{
    node.condition->Accept(*this);
    llvm::Value *conditionValue = m_LastValue;

    if (!conditionValue)
    {
        JLANG_ERROR("Invalid condition in if statement");
        return;
    }

    if (conditionValue->getType()->isIntegerTy(32))
    {
        conditionValue = m_IRBuilder.CreateICmpNE(
            conditionValue, llvm::ConstantInt::get(conditionValue->getType(), 0), "ifcond");
    }

    llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();

    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(m_Context, "then", parentFunction);
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(m_Context, "else");
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(m_Context, "ifcont");

    m_IRBuilder.CreateCondBr(conditionValue, thenBlock, elseBlock);

    m_IRBuilder.SetInsertPoint(thenBlock);
    node.thenBranch->Accept(*this);
    m_IRBuilder.CreateBr(mergeBlock);

    elseBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(elseBlock);
    if (node.elseBranch)
    {
        node.elseBranch->Accept(*this);
    }
    m_IRBuilder.CreateBr(mergeBlock);

    mergeBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(mergeBlock);
}

void CodeGenerator::VisitWhileStatement(WhileStatement &node)
{
    llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();

    llvm::BasicBlock *condBlock = llvm::BasicBlock::Create(m_Context, "while.cond", parentFunction);
    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(m_Context, "while.body");
    llvm::BasicBlock *exitBlock = llvm::BasicBlock::Create(m_Context, "while.exit");

    // Branch to condition block
    m_IRBuilder.CreateBr(condBlock);

    // Condition block
    m_IRBuilder.SetInsertPoint(condBlock);
    node.condition->Accept(*this);
    llvm::Value *condValue = m_LastValue;

    if (!condValue)
    {
        JLANG_ERROR("Invalid condition in while statement");
        return;
    }

    // Convert i32 to i1 if necessary
    if (condValue->getType()->isIntegerTy(32))
    {
        condValue =
            m_IRBuilder.CreateICmpNE(condValue, llvm::ConstantInt::get(condValue->getType(), 0), "whilecond");
    }

    m_IRBuilder.CreateCondBr(condValue, bodyBlock, exitBlock);

    // Body block
    bodyBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(bodyBlock);
    m_LoopExitStack.push_back(exitBlock);
    m_LoopContinueStack.push_back(condBlock);
    node.body->Accept(*this);
    m_LoopContinueStack.pop_back();
    m_LoopExitStack.pop_back();
    if (!m_IRBuilder.GetInsertBlock()->getTerminator())
    {
        m_IRBuilder.CreateBr(condBlock);
    }

    // Exit block
    exitBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(exitBlock);
}

void CodeGenerator::VisitForStatement(ForStatement &node)
{
    // Execute initializer (if present)
    if (node.init)
    {
        node.init->Accept(*this);
    }

    llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();

    llvm::BasicBlock *condBlock = llvm::BasicBlock::Create(m_Context, "for.cond", parentFunction);
    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(m_Context, "for.body");
    llvm::BasicBlock *updateBlock = llvm::BasicBlock::Create(m_Context, "for.update");
    llvm::BasicBlock *exitBlock = llvm::BasicBlock::Create(m_Context, "for.exit");

    // Branch to condition block
    m_IRBuilder.CreateBr(condBlock);

    // Condition block
    m_IRBuilder.SetInsertPoint(condBlock);
    if (node.condition)
    {
        node.condition->Accept(*this);
        llvm::Value *condValue = m_LastValue;

        if (!condValue)
        {
            JLANG_ERROR("Invalid condition in for statement");
            return;
        }

        // Convert i32 to i1 if necessary
        if (condValue->getType()->isIntegerTy(32))
        {
            condValue = m_IRBuilder.CreateICmpNE(condValue, llvm::ConstantInt::get(condValue->getType(), 0),
                                                 "forcond");
        }

        m_IRBuilder.CreateCondBr(condValue, bodyBlock, exitBlock);
    }
    else
    {
        // No condition = infinite loop
        m_IRBuilder.CreateBr(bodyBlock);
    }

    // Body block
    bodyBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(bodyBlock);
    m_LoopExitStack.push_back(exitBlock);
    m_LoopContinueStack.push_back(updateBlock);
    node.body->Accept(*this);
    m_LoopContinueStack.pop_back();
    m_LoopExitStack.pop_back();
    if (!m_IRBuilder.GetInsertBlock()->getTerminator())
    {
        m_IRBuilder.CreateBr(updateBlock);
    }

    // Update block
    updateBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(updateBlock);
    if (node.update)
    {
        node.update->Accept(*this);
    }
    m_IRBuilder.CreateBr(condBlock);

    // Exit block
    exitBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(exitBlock);
}

void CodeGenerator::VisitForEachStatement(ForEachStatement &node)
{
    // Resolve the iterable variable
    std::string varName;
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.iterable.get()))
    {
        varName = varExpr->name;
    }

    if (varName.empty())
    {
        JLANG_ERROR("foreach requires a variable as the iterable");
        return;
    }

    VariableInfo *iterInfo = m_symbols.LookupVariable(varName);
    if (!iterInfo)
    {
        JLANG_ERROR(STR("Undefined variable in foreach: %s", varName.c_str()));
        return;
    }
    iterInfo->used = true;

    llvm::Type *i64Type = llvm::Type::getInt64Ty(m_Context);
    llvm::Type *elemType = nullptr;
    llvm::Value *lengthVal = nullptr;

    // Determine collection type and get element type + length
    bool isArray = iterInfo->type.isArrayType();
    bool isVector = iterInfo->type.isVector();

    if (isArray)
    {
        TypeRef elemRef;
        elemRef.name = iterInfo->type.name;
        elemType = MapType(elemRef);
        lengthVal = llvm::ConstantInt::get(i64Type, iterInfo->type.arraySize);
    }
    else if (isVector)
    {
        VectorTypeInfo *vecInfo = m_symbols.LookupVectorType(iterInfo->type.getMangledName());
        if (!vecInfo)
        {
            JLANG_ERROR("Cannot find vector type info for foreach");
            return;
        }
        elemType = MapType(vecInfo->elementType);

        // Load size from vector struct (field index 1)
        llvm::AllocaInst *vecAlloca = llvm::dyn_cast<llvm::AllocaInst>(iterInfo->value);
        llvm::Value *sizePtr =
            m_IRBuilder.CreateStructGEP(vecInfo->llvmType, vecAlloca, 1, "foreach_size_ptr");
        lengthVal = m_IRBuilder.CreateLoad(i64Type, sizePtr, "foreach_len");
    }
    else
    {
        JLANG_ERROR("foreach requires an array or vector iterable");
        return;
    }

    llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();

    // Create index variable: i = 0
    llvm::AllocaInst *idxAlloca = m_IRBuilder.CreateAlloca(i64Type, nullptr, "foreach_idx");
    m_IRBuilder.CreateStore(llvm::ConstantInt::get(i64Type, 0), idxAlloca);

    // Create element variable alloca
    llvm::AllocaInst *elemAlloca = m_IRBuilder.CreateAlloca(elemType, nullptr, node.elementName);

    // Basic blocks
    llvm::BasicBlock *condBlock = llvm::BasicBlock::Create(m_Context, "foreach.cond", parentFunction);
    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(m_Context, "foreach.body");
    llvm::BasicBlock *updateBlock = llvm::BasicBlock::Create(m_Context, "foreach.update");
    llvm::BasicBlock *exitBlock = llvm::BasicBlock::Create(m_Context, "foreach.exit");

    m_IRBuilder.CreateBr(condBlock);

    // Condition: i < length
    m_IRBuilder.SetInsertPoint(condBlock);
    llvm::Value *idx = m_IRBuilder.CreateLoad(i64Type, idxAlloca, "i");

    // For vectors, reload length each iteration (size may not change but this is correct)
    llvm::Value *len = lengthVal;
    if (isVector)
    {
        VectorTypeInfo *vecInfo = m_symbols.LookupVectorType(iterInfo->type.getMangledName());
        llvm::AllocaInst *vecAlloca = llvm::dyn_cast<llvm::AllocaInst>(iterInfo->value);
        llvm::Value *sizePtr =
            m_IRBuilder.CreateStructGEP(vecInfo->llvmType, vecAlloca, 1, "foreach_size_ptr");
        len = m_IRBuilder.CreateLoad(i64Type, sizePtr, "foreach_len");
    }

    llvm::Value *cond = m_IRBuilder.CreateICmpSLT(idx, len, "foreach_cond");
    m_IRBuilder.CreateCondBr(cond, bodyBlock, exitBlock);

    // Body: elem = collection[i]; execute body
    bodyBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(bodyBlock);

    // Load element at index i
    llvm::Value *elemVal = nullptr;
    if (isArray)
    {
        llvm::AllocaInst *arrAlloca = llvm::dyn_cast<llvm::AllocaInst>(iterInfo->value);
        llvm::Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), 0);
        llvm::Value *elemPtr =
            m_IRBuilder.CreateGEP(arrAlloca->getAllocatedType(), arrAlloca, {zero, idx}, "foreach_elem_ptr");
        elemVal = m_IRBuilder.CreateLoad(elemType, elemPtr, "foreach_elem");
    }
    else if (isVector)
    {
        VectorTypeInfo *vecInfo = m_symbols.LookupVectorType(iterInfo->type.getMangledName());
        llvm::AllocaInst *vecAlloca = llvm::dyn_cast<llvm::AllocaInst>(iterInfo->value);
        llvm::Type *elemPtrType = llvm::PointerType::getUnqual(elemType);
        llvm::Value *dataFieldPtr =
            m_IRBuilder.CreateStructGEP(vecInfo->llvmType, vecAlloca, 0, "foreach_data_ptr");
        llvm::Value *dataPtr = m_IRBuilder.CreateLoad(elemPtrType, dataFieldPtr, "foreach_data");
        llvm::Value *elemPtr = m_IRBuilder.CreateGEP(elemType, dataPtr, idx, "foreach_vec_elem_ptr");
        elemVal = m_IRBuilder.CreateLoad(elemType, elemPtr, "foreach_elem");
    }

    m_IRBuilder.CreateStore(elemVal, elemAlloca);

    // Register element variable in symbol table
    TypeRef elemTypeRef;
    if (isArray)
    {
        elemTypeRef.name = iterInfo->type.name;
    }
    else if (isVector)
    {
        VectorTypeInfo *vecInfo = m_symbols.LookupVectorType(iterInfo->type.getMangledName());
        elemTypeRef = vecInfo->elementType;
    }
    m_symbols.DefineVariable(node.elementName, VariableInfo{elemAlloca, elemTypeRef, false, false});
    m_symbols.TrackFunctionLocal(node.elementName);

    // Push loop stacks for break/continue
    m_LoopExitStack.push_back(exitBlock);
    m_LoopContinueStack.push_back(updateBlock);
    node.body->Accept(*this);
    m_LoopContinueStack.pop_back();
    m_LoopExitStack.pop_back();

    if (!m_IRBuilder.GetInsertBlock()->getTerminator())
    {
        m_IRBuilder.CreateBr(updateBlock);
    }

    // Update: i++
    updateBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(updateBlock);
    llvm::Value *nextIdx = m_IRBuilder.CreateAdd(m_IRBuilder.CreateLoad(i64Type, idxAlloca, "i_cur"),
                                                 llvm::ConstantInt::get(i64Type, 1), "i_next");
    m_IRBuilder.CreateStore(nextIdx, idxAlloca);
    m_IRBuilder.CreateBr(condBlock);

    // Exit
    exitBlock->insertInto(parentFunction);
    m_IRBuilder.SetInsertPoint(exitBlock);
}

void CodeGenerator::VisitBlockStatement(BlockStatement &node)
{
    for (auto &statement : node.statements)
    {
        if (statement)
        {
            statement->Accept(*this);
        }
    }
}

void CodeGenerator::VisitExprStatement(ExprStatement &node)
{
    if (node.expression)
    {
        node.expression->Accept(*this);
    }
}

void CodeGenerator::VisitReturnStatement(ReturnStatement &node)
{
    if (node.value)
    {
        node.value->Accept(*this);
        m_IRBuilder.CreateRet(m_LastValue);
    }
    else
    {
        m_IRBuilder.CreateRetVoid();
    }
}

void CodeGenerator::VisitBreakStatement(BreakStatement &)
{
    if (m_LoopExitStack.empty())
    {
        JLANG_ERROR("'break' used outside of a loop");
        return;
    }

    m_IRBuilder.CreateBr(m_LoopExitStack.back());

    // Create unreachable block for any dead code after break
    llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();
    llvm::BasicBlock *deadBlock = llvm::BasicBlock::Create(m_Context, "break.dead", parentFunction);
    m_IRBuilder.SetInsertPoint(deadBlock);
}

void CodeGenerator::VisitContinueStatement(ContinueStatement &)
{
    if (m_LoopContinueStack.empty())
    {
        JLANG_ERROR("'continue' used outside of a loop");
        return;
    }

    m_IRBuilder.CreateBr(m_LoopContinueStack.back());

    // Create unreachable block for any dead code after continue
    llvm::Function *parentFunction = m_IRBuilder.GetInsertBlock()->getParent();
    llvm::BasicBlock *deadBlock = llvm::BasicBlock::Create(m_Context, "continue.dead", parentFunction);
    m_IRBuilder.SetInsertPoint(deadBlock);
}

} // namespace jlang
