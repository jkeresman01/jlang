#include "LoopUnroller.h"

namespace jlang
{

std::optional<LoopUnrollInfo> LoopUnroller::AnalyzeForUnroll(ForStatement &node)
{
    // 1. Init must be a VariableDecl with a LiteralExpr integer initializer
    if (!node.init || !node.condition || !node.update || !node.body)
        return std::nullopt;

    auto *varDecl = dynamic_cast<VariableDecl *>(node.init.get());
    if (!varDecl || !varDecl->initializer)
        return std::nullopt;

    auto *initLiteral = dynamic_cast<LiteralExpr *>(varDecl->initializer.get());
    if (!initLiteral)
        return std::nullopt;

    int64_t startVal;
    try
    {
        startVal = std::stoll(initLiteral->value);
    }
    catch (...)
    {
        return std::nullopt;
    }

    std::string varName = varDecl->name;

    // 2. Condition must be BinaryExpr comparing the loop variable to a literal
    auto *condExpr = dynamic_cast<BinaryExpr *>(node.condition.get());
    if (!condExpr)
        return std::nullopt;

    std::string compareOp = condExpr->op;
    if (compareOp != "<" && compareOp != "<=" && compareOp != ">" && compareOp != ">=")
        return std::nullopt;

    // Check that one side is the loop variable and the other is a literal
    auto *leftVar = dynamic_cast<VarExpr *>(condExpr->left.get());
    auto *rightLit = dynamic_cast<LiteralExpr *>(condExpr->right.get());

    if (!leftVar || leftVar->name != varName || !rightLit)
        return std::nullopt;

    int64_t endVal;
    try
    {
        endVal = std::stoll(rightLit->value);
    }
    catch (...)
    {
        return std::nullopt;
    }

    // 3. Update must be PostfixExpr or PrefixExpr with ++ or -- on the loop variable
    int64_t step = 0;
    if (auto *postfix = dynamic_cast<PostfixExpr *>(node.update.get()))
    {
        auto *operandVar = dynamic_cast<VarExpr *>(postfix->operand.get());
        if (!operandVar || operandVar->name != varName)
            return std::nullopt;
        if (postfix->op == "++")
            step = 1;
        else if (postfix->op == "--")
            step = -1;
        else
            return std::nullopt;
    }
    else if (auto *prefix = dynamic_cast<PrefixExpr *>(node.update.get()))
    {
        auto *operandVar = dynamic_cast<VarExpr *>(prefix->operand.get());
        if (!operandVar || operandVar->name != varName)
            return std::nullopt;
        if (prefix->op == "++")
            step = 1;
        else if (prefix->op == "--")
            step = -1;
        else
            return std::nullopt;
    }
    else
    {
        return std::nullopt;
    }

    // 4. Compute trip count
    int64_t tripCount = 0;
    if (step == 1)
    {
        if (compareOp == "<")
            tripCount = endVal - startVal;
        else if (compareOp == "<=")
            tripCount = endVal - startVal + 1;
        else
            return std::nullopt; // step=+1 with > or >= makes no sense
    }
    else // step == -1
    {
        if (compareOp == ">")
            tripCount = startVal - endVal;
        else if (compareOp == ">=")
            tripCount = startVal - endVal + 1;
        else
            return std::nullopt; // step=-1 with < or <= makes no sense
    }

    if (tripCount < 1 || tripCount > MAX_UNROLL_COUNT)
        return std::nullopt;

    return LoopUnrollInfo{varName, startVal, endVal, step, compareOp};
}

bool LoopUnroller::BodyModifiesVar(const std::shared_ptr<AstNode> &node, const std::string &varName)
{
    if (!node)
        return false;

    // Check AssignExpr
    if (auto *assign = dynamic_cast<AssignExpr *>(node.get()))
    {
        if (assign->name == varName)
            return true;
        return BodyModifiesVar(assign->value, varName);
    }

    // Check PrefixExpr (++/-- on the variable)
    if (auto *prefix = dynamic_cast<PrefixExpr *>(node.get()))
    {
        if (auto *varExpr = dynamic_cast<VarExpr *>(prefix->operand.get()))
        {
            if (varExpr->name == varName)
                return true;
        }
        return BodyModifiesVar(prefix->operand, varName);
    }

    // Check PostfixExpr (++/-- on the variable)
    if (auto *postfix = dynamic_cast<PostfixExpr *>(node.get()))
    {
        if (auto *varExpr = dynamic_cast<VarExpr *>(postfix->operand.get()))
        {
            if (varExpr->name == varName)
                return true;
        }
        return BodyModifiesVar(postfix->operand, varName);
    }

    // Recurse into compound statements
    if (auto *block = dynamic_cast<BlockStatement *>(node.get()))
    {
        for (auto &stmt : block->statements)
        {
            if (BodyModifiesVar(stmt, varName))
                return true;
        }
        return false;
    }

    if (auto *exprStmt = dynamic_cast<ExprStatement *>(node.get()))
    {
        return BodyModifiesVar(exprStmt->expression, varName);
    }

    if (auto *ifStmt = dynamic_cast<IfStatement *>(node.get()))
    {
        return BodyModifiesVar(ifStmt->condition, varName) || BodyModifiesVar(ifStmt->thenBranch, varName) ||
               BodyModifiesVar(ifStmt->elseBranch, varName);
    }

    if (auto *whileStmt = dynamic_cast<WhileStatement *>(node.get()))
    {
        return BodyModifiesVar(whileStmt->condition, varName) || BodyModifiesVar(whileStmt->body, varName);
    }

    if (auto *forStmt = dynamic_cast<ForStatement *>(node.get()))
    {
        return BodyModifiesVar(forStmt->init, varName) || BodyModifiesVar(forStmt->condition, varName) ||
               BodyModifiesVar(forStmt->update, varName) || BodyModifiesVar(forStmt->body, varName);
    }

    if (auto *callExpr = dynamic_cast<CallExpr *>(node.get()))
    {
        for (auto &arg : callExpr->arguments)
        {
            if (BodyModifiesVar(arg, varName))
                return true;
        }
        return false;
    }

    if (auto *binExpr = dynamic_cast<BinaryExpr *>(node.get()))
    {
        return BodyModifiesVar(binExpr->left, varName) || BodyModifiesVar(binExpr->right, varName);
    }

    if (auto *unaryExpr = dynamic_cast<UnaryExpr *>(node.get()))
    {
        return BodyModifiesVar(unaryExpr->operand, varName);
    }

    if (auto *castExpr = dynamic_cast<CastExpr *>(node.get()))
    {
        return BodyModifiesVar(castExpr->expr, varName);
    }

    if (auto *retStmt = dynamic_cast<ReturnStatement *>(node.get()))
    {
        return BodyModifiesVar(retStmt->value, varName);
    }

    // VarExpr, LiteralExpr, etc. don't modify anything
    return false;
}

} // namespace jlang
