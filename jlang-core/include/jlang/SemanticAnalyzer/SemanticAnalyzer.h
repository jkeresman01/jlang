#pragma once

#include <jlang/AST/Ast.h>
#include <jlang/AST/Expressions/Expressions.h>
#include <jlang/AST/Statements/Statements.h>
#include <jlang/AST/TopLevelDecl/TopLevelDecl.h>
#include <jlang/CodeGen/AstVisitor.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace jlang
{

class SemanticAnalyzer : public AstVisitor
{
  public:
    void Analyze(std::vector<std::shared_ptr<AstNode>> &program);

  private:
    void VisitFunctionDecl(FunctionDecl &) override;
    void VisitInterfaceDecl(InterfaceDecl &) override;
    void VisitStructDecl(StructDecl &) override;
    void VisitVariableDecl(VariableDecl &) override;

    void VisitIfStatement(IfStatement &) override;
    void VisitWhileStatement(WhileStatement &) override;
    void VisitForStatement(ForStatement &) override;
    void VisitForEachStatement(ForEachStatement &) override;
    void VisitBlockStatement(BlockStatement &) override;
    void VisitExprStatement(ExprStatement &) override;
    void VisitReturnStatement(ReturnStatement &) override;
    void VisitBreakStatement(BreakStatement &) override;
    void VisitContinueStatement(ContinueStatement &) override;

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
    void VisitArrayLiteralExpr(ArrayLiteralExpr &) override;
    void VisitIndexExpr(IndexExpr &) override;
    void VisitIndexAssignExpr(IndexAssignExpr &) override;
    void VisitMethodCallExpr(MethodCallExpr &) override;
    void VisitMemberAssignExpr(MemberAssignExpr &) override;

    void CheckUnusedVariables();
    void ValidateInterfaceImplementations();

  private:
    std::unordered_map<std::string, bool> m_declaredVariables; // name -> used
    std::unordered_set<std::string> m_currentFunctionVariables;
    int m_LoopDepth = 0;

    // Interface/struct/function registries for validation
    std::unordered_map<std::string, std::vector<InterfaceMethodDecl>> m_declaredInterfaces;
    std::unordered_map<std::string, std::string> m_structInterfaces; // struct -> interface
    // function key: "StructName_methodName", value: {params, returnType}
    struct FuncSig
    {
        std::vector<Parameter> params;
        TypeRef returnType;
    };
    std::unordered_map<std::string, FuncSig> m_functionSignatures;
};

} // namespace jlang
