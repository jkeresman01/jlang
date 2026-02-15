#pragma once

#include <jlang/CodeGen/AstVisitor.h>

#include <memory>
#include <string>
#include <vector>

namespace jlang
{

enum class NodeType
{
    InterfaceDecl,
    StructDecl,
    FunctionDecl,
    VariableDecl,

    IfStatement,
    WhileStatement,
    ForStatement,
    BlockStatement,
    ExprStatement,
    ReturnStatement,
    BreakStatement,
    ContinueStatement,

    CallExpr,
    BinaryExpr,
    UnaryExpr,
    VarExpr,
    LiteralExpr,
    CastExpr,
    AllocExpr,
    AssignExpr,
    MemberAccessExpr,
    PrefixExpr,
    PostfixExpr,
    MatchExpr,
    OkExpr,
    ErrExpr,
    ArrayLiteralExpr,
    IndexExpr,
    IndexAssignExpr,
    MethodCallExpr,
    MemberAssignExpr
};

struct AstNode
{
    NodeType type;

    virtual ~AstNode() = default;

    virtual void Accept(AstVisitor &visitor) = 0;
};

using AstNodePtr = std::shared_ptr<AstNode>;

} // namespace jlang
