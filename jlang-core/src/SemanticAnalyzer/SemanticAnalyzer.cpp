#include <jlang/SemanticAnalyzer/SemanticAnalyzer.h>

#include <iostream>

namespace jlang
{

void SemanticAnalyzer::Analyze(std::vector<std::shared_ptr<AstNode>> &program)
{
    // Pass 1: collect all declarations
    for (auto &node : program)
    {
        if (node)
        {
            node->Accept(*this);
        }
    }

    // Pass 2: validate interface implementations
    ValidateInterfaceImplementations();
}

void SemanticAnalyzer::VisitFunctionDecl(FunctionDecl &node)
{
    // Skip generic function templates — they contain unresolved type parameters
    if (!node.typeParameters.empty())
        return;

    // Record function signature for interface validation
    // If first param is self: StructName*, register as StructName_methodName
    if (!node.params.empty() && node.params[0].name == "self" && node.params[0].type.isPointer)
    {
        std::string structName = node.params[0].type.name;
        std::string key = structName + "_" + node.name;
        FuncSig sig;
        // Store params excluding self
        for (size_t i = 1; i < node.params.size(); ++i)
            sig.params.push_back(node.params[i]);
        sig.returnType = node.returnType;
        m_functionSignatures[key] = sig;
    }

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

void SemanticAnalyzer::VisitInterfaceDecl(InterfaceDecl &node)
{
    m_declaredInterfaces[node.name] = node.methods;
}

void SemanticAnalyzer::VisitStructDecl(StructDecl &node)
{
    // Skip generic struct templates
    if (!node.typeParameters.empty())
        return;

    if (!node.interfaceImplemented.empty())
    {
        m_structInterfaces[node.name] = node.interfaceImplemented;
    }
}

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

void SemanticAnalyzer::VisitForEachStatement(ForEachStatement &node)
{
    if (node.iterable)
        node.iterable->Accept(*this);
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

void SemanticAnalyzer::VisitSwitchStatement(SwitchStatement &node)
{
    if (node.expr)
        node.expr->Accept(*this);
    m_SwitchDepth++;
    for (auto &c : node.cases)
    {
        for (auto &v : c.values)
        {
            if (v)
                v->Accept(*this);
        }
        if (c.body)
            c.body->Accept(*this);
    }
    m_SwitchDepth--;
}

void SemanticAnalyzer::VisitBreakStatement(BreakStatement &)
{
    if (m_LoopDepth == 0 && m_SwitchDepth == 0)
    {
        std::cerr << "Error: 'break' used outside of a loop or switch" << std::endl;
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

void SemanticAnalyzer::VisitArrayLiteralExpr(ArrayLiteralExpr &node)
{
    for (auto &elem : node.elements)
    {
        if (elem)
            elem->Accept(*this);
    }
}

void SemanticAnalyzer::VisitIndexExpr(IndexExpr &node)
{
    if (node.object)
        node.object->Accept(*this);
    if (node.index)
        node.index->Accept(*this);
}

void SemanticAnalyzer::VisitIndexAssignExpr(IndexAssignExpr &node)
{
    // Mark the base variable as used
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.object.get()))
    {
        auto it = m_declaredVariables.find(varExpr->name);
        if (it != m_declaredVariables.end())
        {
            it->second = true;
        }
    }

    if (node.object)
        node.object->Accept(*this);
    if (node.index)
        node.index->Accept(*this);
    if (node.value)
        node.value->Accept(*this);
}

void SemanticAnalyzer::VisitMethodCallExpr(MethodCallExpr &node)
{
    if (node.object)
        node.object->Accept(*this);
    for (auto &arg : node.arguments)
    {
        if (arg)
            arg->Accept(*this);
    }
}

void SemanticAnalyzer::VisitMemberAssignExpr(MemberAssignExpr &node)
{
    if (node.object)
        node.object->Accept(*this);
    if (node.value)
        node.value->Accept(*this);
}

void SemanticAnalyzer::VisitSwitchExpr(SwitchExpr &node)
{
    if (node.expr)
        node.expr->Accept(*this);
    for (auto &arm : node.arms)
    {
        for (auto &v : arm.values)
        {
            if (v)
                v->Accept(*this);
        }
        if (arm.body)
            arm.body->Accept(*this);
    }
}

void SemanticAnalyzer::ValidateInterfaceImplementations()
{
    for (auto &[structName, ifaceName] : m_structInterfaces)
    {
        auto ifaceIt = m_declaredInterfaces.find(ifaceName);
        if (ifaceIt == m_declaredInterfaces.end())
        {
            std::cerr << "Error: Struct '" << structName << "' implements unknown interface '" << ifaceName
                      << "'" << std::endl;
            continue;
        }

        for (auto &method : ifaceIt->second)
        {
            std::string key = structName + "_" + method.name;
            auto funcIt = m_functionSignatures.find(key);
            if (funcIt == m_functionSignatures.end())
            {
                std::cerr << "Error: Struct '" << structName << "' does not implement method '" << method.name
                          << "' required by interface '" << ifaceName << "'" << std::endl;
                continue;
            }

            // Check parameter count (excluding self)
            if (funcIt->second.params.size() != method.params.size())
            {
                std::cerr << "Error: Method '" << method.name << "' in struct '" << structName
                          << "' has wrong number of parameters for interface '" << ifaceName << "'"
                          << std::endl;
            }

            // Check return type
            if (funcIt->second.returnType.name != method.returnType.name)
            {
                std::cerr << "Error: Method '" << method.name << "' in struct '" << structName
                          << "' has wrong return type for interface '" << ifaceName << "'" << std::endl;
            }
        }
    }
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
