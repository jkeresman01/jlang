#include "../CodeGen.h"

#include "../../Common/Logger.h"

namespace jlang
{

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
    else if (node.op == "-")
    {
        // Unary negation
        if (operandVal->getType()->isFloatingPointTy())
        {
            m_LastValue = m_IRBuilder.CreateFNeg(operandVal, "neg");
        }
        else
        {
            m_LastValue = m_IRBuilder.CreateNeg(operandVal, "neg");
        }
    }
    else
    {
        JLANG_ERROR(STR("Unknown unary operator: %s", node.op.c_str()));
    }
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
