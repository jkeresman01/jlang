#include "../CodeGen.h"

#include "../../Common/Logger.h"

namespace jlang
{

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

} // namespace jlang
