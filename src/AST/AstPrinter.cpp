#include "AstPrinter.h"
#include "Expressions/Expressions.h"
#include "Statements/Statements.h"
#include "TopLevelDecl/TopLevelDecl.h"

namespace jlang
{

std::string AstPrinter::Print(std::vector<std::shared_ptr<AstNode>> &program)
{
    m_result.clear();
    m_indent = 0;

    for (auto &node : program)
    {
        if (node)
            node->Accept(*this);
        else
            m_result += "<null>\n";
    }

    return m_result;
}

void AstPrinter::Indent()
{
    for (int i = 0; i < m_indent; ++i)
        m_result += "  ";
}

void AstPrinter::VisitChild(std::shared_ptr<AstNode> &node)
{
    if (node)
    {
        node->Accept(*this);
    }
    else
    {
        Indent();
        m_result += "<null>\n";
    }
}

std::string AstPrinter::FormatTypeRef(const TypeRef &typeRef)
{
    std::string s = typeRef.name;
    if (typeRef.isPointer)
        s += "*";
    if (typeRef.isNullable)
        s += "?";
    return s;
}

void AstPrinter::VisitFunctionDecl(FunctionDecl &node)
{
    Indent();
    m_result += "(FunctionDecl " + node.name + " [";
    for (size_t i = 0; i < node.params.size(); ++i)
    {
        if (i > 0)
            m_result += ", ";
        m_result += node.params[i].name + ":" + FormatTypeRef(node.params[i].type);
    }
    m_result += "] -> " + FormatTypeRef(node.returnType) + "\n";

    ++m_indent;
    VisitChild(node.body);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitInterfaceDecl(InterfaceDecl &node)
{
    Indent();
    m_result += "(InterfaceDecl " + node.name + " [";
    for (size_t i = 0; i < node.methods.size(); ++i)
    {
        if (i > 0)
            m_result += ", ";
        m_result += node.methods[i];
    }
    m_result += "])\n";
}

void AstPrinter::VisitStructDecl(StructDecl &node)
{
    Indent();
    m_result += "(StructDecl " + node.name;
    if (!node.interfaceImplemented.empty())
        m_result += " implements " + node.interfaceImplemented;
    m_result += " [";
    for (size_t i = 0; i < node.fields.size(); ++i)
    {
        if (i > 0)
            m_result += ", ";
        m_result += node.fields[i].name + ":" + FormatTypeRef(node.fields[i].type);
        if (node.fields[i].isPublic)
            m_result += " pub";
    }
    m_result += "])\n";
}

void AstPrinter::VisitVariableDecl(VariableDecl &node)
{
    Indent();
    m_result += "(VariableDecl " + node.name + " : " + FormatTypeRef(node.varType);
    m_result += (node.isMutable ? " mut" : " immut");
    m_result += "\n";

    ++m_indent;
    VisitChild(node.initializer);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitIfStatement(IfStatement &node)
{
    Indent();
    m_result += "(IfStatement\n";

    ++m_indent;
    VisitChild(node.condition);
    VisitChild(node.thenBranch);
    if (node.elseBranch)
        VisitChild(node.elseBranch);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitWhileStatement(WhileStatement &node)
{
    Indent();
    m_result += "(WhileStatement\n";

    ++m_indent;
    VisitChild(node.condition);
    VisitChild(node.body);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitForStatement(ForStatement &node)
{
    Indent();
    m_result += "(ForStatement\n";

    ++m_indent;
    VisitChild(node.init);
    VisitChild(node.condition);
    VisitChild(node.update);
    VisitChild(node.body);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitBlockStatement(BlockStatement &node)
{
    Indent();
    m_result += "(BlockStatement\n";

    ++m_indent;
    for (auto &stmt : node.statements)
        VisitChild(stmt);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitExprStatement(ExprStatement &node)
{
    Indent();
    m_result += "(ExprStatement\n";

    ++m_indent;
    VisitChild(node.expression);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitReturnStatement(ReturnStatement &node)
{
    Indent();
    m_result += "(ReturnStatement\n";

    ++m_indent;
    VisitChild(node.value);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitBreakStatement(BreakStatement &)
{
    Indent();
    m_result += "(BreakStatement)\n";
}

void AstPrinter::VisitContinueStatement(ContinueStatement &)
{
    Indent();
    m_result += "(ContinueStatement)\n";
}

void AstPrinter::VisitCallExpr(CallExpr &node)
{
    Indent();
    m_result += "(CallExpr " + node.callee + "\n";

    ++m_indent;
    for (auto &arg : node.arguments)
        VisitChild(arg);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitBinaryExpr(BinaryExpr &node)
{
    Indent();
    m_result += "(BinaryExpr " + node.op + "\n";

    ++m_indent;
    VisitChild(node.left);
    VisitChild(node.right);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitUnaryExpr(UnaryExpr &node)
{
    Indent();
    m_result += "(UnaryExpr " + node.op + "\n";

    ++m_indent;
    VisitChild(node.operand);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitLiteralExpr(LiteralExpr &node)
{
    Indent();
    m_result += "(LiteralExpr " + node.value + ")\n";
}

void AstPrinter::VisitVarExpr(VarExpr &node)
{
    Indent();
    m_result += "(VarExpr " + node.name + ")\n";
}

void AstPrinter::VisitCastExpr(CastExpr &node)
{
    Indent();
    m_result += "(CastExpr -> " + FormatTypeRef(node.targetType) + "\n";

    ++m_indent;
    VisitChild(node.expr);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitAllocExpr(AllocExpr &node)
{
    Indent();
    m_result += "(AllocExpr " + FormatTypeRef(node.allocType) + ")\n";
}

void AstPrinter::VisitAssignExpr(AssignExpr &node)
{
    Indent();
    m_result += "(AssignExpr " + node.name + "\n";

    ++m_indent;
    VisitChild(node.value);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitMemberAccessExpr(MemberAccessExpr &node)
{
    Indent();
    m_result += "(MemberAccessExpr ." + node.memberName + "\n";

    ++m_indent;
    VisitChild(node.object);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitPrefixExpr(PrefixExpr &node)
{
    Indent();
    m_result += "(PrefixExpr " + node.op + "\n";

    ++m_indent;
    VisitChild(node.operand);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitPostfixExpr(PostfixExpr &node)
{
    Indent();
    m_result += "(PostfixExpr " + node.op + "\n";

    ++m_indent;
    VisitChild(node.operand);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitMatchExpr(MatchExpr &node)
{
    Indent();
    m_result += "(MatchExpr\n";

    ++m_indent;
    Indent();
    m_result += "scrutinee:\n";
    ++m_indent;
    VisitChild(node.scrutinee);
    --m_indent;

    Indent();
    m_result += "Ok(" + node.okArm.bindingName + ") =>\n";
    ++m_indent;
    VisitChild(node.okArm.body);
    --m_indent;

    Indent();
    m_result += "Err(" + node.errArm.bindingName + ") =>\n";
    ++m_indent;
    VisitChild(node.errArm.body);
    --m_indent;

    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitOkExpr(OkExpr &node)
{
    Indent();
    m_result += "(OkExpr\n";

    ++m_indent;
    VisitChild(node.value);
    --m_indent;

    Indent();
    m_result += ")\n";
}

void AstPrinter::VisitErrExpr(ErrExpr &node)
{
    Indent();
    m_result += "(ErrExpr\n";

    ++m_indent;
    VisitChild(node.error);
    --m_indent;

    Indent();
    m_result += ")\n";
}

} // namespace jlang
