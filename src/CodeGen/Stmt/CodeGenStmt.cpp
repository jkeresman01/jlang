#include "../CodeGen.h"

#include "../../Common/Logger.h"

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
    node.body->Accept(*this);
    m_IRBuilder.CreateBr(condBlock);

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
    node.body->Accept(*this);
    m_IRBuilder.CreateBr(updateBlock);

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

} // namespace jlang
