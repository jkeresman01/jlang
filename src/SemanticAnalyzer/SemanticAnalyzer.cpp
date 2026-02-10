#include "SemanticAnalyzer.h"

#include <iostream>

namespace jlang
{

void SemanticAnalyzer::Analyze(std::vector<std::shared_ptr<AstNode>> &program)
{
    for (auto &node : program)
    {
        if (node)
        {
            node->Accept(*this);
        }
    }
}

void SemanticAnalyzer::VisitFunctionDecl(FunctionDecl &node)
{
    m_currentFunctionVariables.clear();
    m_declaredVariables.clear();

    // Track parameters as declared+used (params are always considered used)
    for (auto &param : node.params)
    {
        m_declaredVariables[param.name] = true;
        m_currentFunctionVariables.insert(param.name);
    }

    if (node.body)
    {
        node.body->Accept(*this);
    }

    CheckUnusedVariables();

    m_currentFunctionVariables.clear();
    m_declaredVariables.clear();
}

void SemanticAnalyzer::VisitInterfaceDecl(InterfaceDecl &) {}

void SemanticAnalyzer::VisitStructDecl(StructDecl &) {}

void SemanticAnalyzer::VisitVariableDecl(VariableDecl &node)
{
    m_declaredVariables[node.name] = false;
    m_currentFunctionVariables.insert(node.name);

    if (node.initializer)
    {
        node.initializer->Accept(*this);
    }
}

void SemanticAnalyzer::VisitIfStatement(IfStatement &node)
{
    if (node.condition)
        node.condition->Accept(*this);
    if (node.thenBranch)
        node.thenBranch->Accept(*this);
    if (node.elseBranch)
        node.elseBranch->Accept(*this);
}

void SemanticAnalyzer::VisitWhileStatement(WhileStatement &node)
{
    if (node.condition)
        node.condition->Accept(*this);
    m_LoopDepth++;
    if (node.body)
        node.body->Accept(*this);
    m_LoopDepth--;
}

void SemanticAnalyzer::VisitForStatement(ForStatement &node)
{
    if (node.init)
        node.init->Accept(*this);
    if (node.condition)
        node.condition->Accept(*this);
    if (node.update)
        node.update->Accept(*this);
    m_LoopDepth++;
    if (node.body)
        node.body->Accept(*this);
    m_LoopDepth--;
}

void SemanticAnalyzer::VisitBlockStatement(BlockStatement &node)
{
    for (auto &stmt : node.statements)
    {
        if (stmt)
            stmt->Accept(*this);
    }
}

void SemanticAnalyzer::VisitExprStatement(ExprStatement &node)
{
    if (node.expression)
        node.expression->Accept(*this);
}

void SemanticAnalyzer::VisitReturnStatement(ReturnStatement &node)
{
    if (node.value)
        node.value->Accept(*this);
}

void SemanticAnalyzer::VisitBreakStatement(BreakStatement &)
{
    if (m_LoopDepth == 0)
    {
        std::cerr << "Error: 'break' used outside of a loop" << std::endl;
    }
}

void SemanticAnalyzer::VisitContinueStatement(ContinueStatement &)
{
    if (m_LoopDepth == 0)
    {
        std::cerr << "Error: 'continue' used outside of a loop" << std::endl;
    }
}

void SemanticAnalyzer::VisitCallExpr(CallExpr &node)
{
    for (auto &arg : node.arguments)
    {
        if (arg)
            arg->Accept(*this);
    }
}

void SemanticAnalyzer::VisitBinaryExpr(BinaryExpr &node)
{
    if (node.left)
        node.left->Accept(*this);
    if (node.right)
        node.right->Accept(*this);
}

void SemanticAnalyzer::VisitUnaryExpr(UnaryExpr &node)
{
    if (node.operand)
        node.operand->Accept(*this);
}

void SemanticAnalyzer::VisitLiteralExpr(LiteralExpr &) {}

void SemanticAnalyzer::VisitVarExpr(VarExpr &node)
{
    auto it = m_declaredVariables.find(node.name);
    if (it != m_declaredVariables.end())
    {
        it->second = true;
    }
}

void SemanticAnalyzer::VisitCastExpr(CastExpr &node)
{
    if (node.expr)
        node.expr->Accept(*this);
}

void SemanticAnalyzer::VisitAllocExpr(AllocExpr &) {}

void SemanticAnalyzer::VisitAssignExpr(AssignExpr &node)
{
    // Mark the variable as used
    auto it = m_declaredVariables.find(node.name);
    if (it != m_declaredVariables.end())
    {
        it->second = true;
    }

    if (node.value)
        node.value->Accept(*this);
}

void SemanticAnalyzer::VisitMemberAccessExpr(MemberAccessExpr &node)
{
    if (node.object)
        node.object->Accept(*this);
}

void SemanticAnalyzer::VisitPrefixExpr(PrefixExpr &node)
{
    if (node.operand)
    {
        // Mark the variable as used
        if (auto *varExpr = dynamic_cast<VarExpr *>(node.operand.get()))
        {
            auto it = m_declaredVariables.find(varExpr->name);
            if (it != m_declaredVariables.end())
            {
                it->second = true;
            }
        }
        node.operand->Accept(*this);
    }
}

void SemanticAnalyzer::VisitPostfixExpr(PostfixExpr &node)
{
    if (node.operand)
    {
        // Mark the variable as used
        if (auto *varExpr = dynamic_cast<VarExpr *>(node.operand.get()))
        {
            auto it = m_declaredVariables.find(varExpr->name);
            if (it != m_declaredVariables.end())
            {
                it->second = true;
            }
        }
        node.operand->Accept(*this);
    }
}

void SemanticAnalyzer::VisitMatchExpr(MatchExpr &node)
{
    if (node.scrutinee)
        node.scrutinee->Accept(*this);

    // The binding names in match arms are local to their arms
    // We don't track them for unused variable checking since they're always used
    if (node.okArm.body)
        node.okArm.body->Accept(*this);
    if (node.errArm.body)
        node.errArm.body->Accept(*this);
}

void SemanticAnalyzer::VisitOkExpr(OkExpr &node)
{
    if (node.value)
        node.value->Accept(*this);
}

void SemanticAnalyzer::VisitErrExpr(ErrExpr &node)
{
    if (node.error)
        node.error->Accept(*this);
}

void SemanticAnalyzer::CheckUnusedVariables()
{
    for (const auto &varName : m_currentFunctionVariables)
    {
        auto it = m_declaredVariables.find(varName);
        if (it != m_declaredVariables.end() && !it->second)
        {
            std::cerr << "Warning: Unused variable: " << varName << std::endl;
        }
    }
}

} // namespace jlang
