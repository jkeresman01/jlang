#pragma once

#include "../CodeGen/AstVisitor.h"
#include "Ast.h"

#include <memory>
#include <string>
#include <vector>

namespace jlang
{

class AstPrinter : public AstVisitor
{
  public:
    std::string Print(std::vector<std::shared_ptr<AstNode>> &program);

  private:
    void VisitFunctionDecl(FunctionDecl &) override;
    void VisitInterfaceDecl(InterfaceDecl &) override;
    void VisitStructDecl(StructDecl &) override;
    void VisitVariableDecl(VariableDecl &) override;

    void VisitIfStatement(IfStatement &) override;
    void VisitWhileStatement(WhileStatement &) override;
    void VisitForStatement(ForStatement &) override;
    void VisitBlockStatement(BlockStatement &) override;
    void VisitExprStatement(ExprStatement &) override;
    void VisitReturnStatement(ReturnStatement &) override;
    void VisitBreakStatement(BreakStatement &) override;

    void VisitCallExpr(CallExpr &) override;
    void VisitBinaryExpr(BinaryExpr &) override;
    void VisitUnaryExpr(UnaryExpr &) override;
    void VisitLiteralExpr(LiteralExpr &) override;
    void VisitVarExpr(VarExpr &) override;
    void VisitCastExpr(CastExpr &) override;
    void VisitAllocExpr(AllocExpr &) override;
    void VisitAssignExpr(AssignExpr &) override;
    void VisitMemberAccessExpr(MemberAccessExpr &) override;
    void VisitPrefixExpr(PrefixExpr &) override;
    void VisitPostfixExpr(PostfixExpr &) override;
    void VisitMatchExpr(MatchExpr &) override;
    void VisitOkExpr(OkExpr &) override;
    void VisitErrExpr(ErrExpr &) override;

  private:
    void Indent();
    void VisitChild(std::shared_ptr<AstNode> &node);
    std::string FormatTypeRef(const struct TypeRef &typeRef);

    std::string m_result;
    int m_indent = 0;
};

} // namespace jlang
