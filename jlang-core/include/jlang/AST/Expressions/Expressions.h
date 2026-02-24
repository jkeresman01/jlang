#pragma once

#include <jlang/AST/Ast.h>
#include <jlang/AST/TypeRef.h>

namespace jlang
{

struct Expression : public AstNode
{
};

struct CallExpr : public Expression
{
    std::string callee;
    std::vector<std::shared_ptr<AstNode>> arguments;
    std::vector<TypeRef> typeArguments;

    CallExpr() { type = NodeType::CallExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitCallExpr(*this); }
};

struct BinaryExpr : public Expression
{
    std::string op;
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;

    BinaryExpr() { type = NodeType::BinaryExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitBinaryExpr(*this); }
};

struct UnaryExpr : public Expression
{
    std::string op;
    std::shared_ptr<AstNode> operand;

    UnaryExpr() { type = NodeType::UnaryExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitUnaryExpr(*this); }
};

struct VarExpr : public Expression
{
    std::string name;

    VarExpr() { type = NodeType::VarExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitVarExpr(*this); }
};

struct LiteralExpr : public Expression
{
    std::string value;

    LiteralExpr() { type = NodeType::LiteralExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitLiteralExpr(*this); }
};

struct CastExpr : public Expression
{
    TypeRef targetType;
    std::shared_ptr<AstNode> expr;

    CastExpr() { type = NodeType::CastExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitCastExpr(*this); }
};

struct AllocExpr : public Expression
{
    TypeRef allocType;

    AllocExpr() { type = NodeType::AllocExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitAllocExpr(*this); }
};

struct AssignExpr : public Expression
{
    std::string name;
    std::shared_ptr<AstNode> value;

    AssignExpr() { type = NodeType::AssignExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitAssignExpr(*this); }
};

struct MemberAccessExpr : public Expression
{
    std::shared_ptr<AstNode> object;
    std::string memberName;

    MemberAccessExpr() { type = NodeType::MemberAccessExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitMemberAccessExpr(*this); }
};

struct PrefixExpr : public Expression
{
    std::string op; // "++" or "--"
    std::shared_ptr<AstNode> operand;

    PrefixExpr() { type = NodeType::PrefixExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitPrefixExpr(*this); }
};

struct PostfixExpr : public Expression
{
    std::string op; // "++" or "--"
    std::shared_ptr<AstNode> operand;

    PostfixExpr() { type = NodeType::PostfixExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitPostfixExpr(*this); }
};

struct OkExpr : public Expression
{
    std::shared_ptr<AstNode> value;
    TypeRef resultType; // Full Result<T, E> type (inferred from context)

    OkExpr() { type = NodeType::OkExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitOkExpr(*this); }
};

struct ErrExpr : public Expression
{
    std::shared_ptr<AstNode> error;
    TypeRef resultType; // Full Result<T, E> type (inferred from context)

    ErrExpr() { type = NodeType::ErrExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitErrExpr(*this); }
};

struct MatchArm
{
    std::string pattern;     // "Ok" or "Err"
    std::string bindingName; // Variable name to bind the extracted value
    std::shared_ptr<AstNode> body;
};

struct MatchExpr : public Expression
{
    std::shared_ptr<AstNode> scrutinee;
    MatchArm okArm;
    MatchArm errArm;

    MatchExpr() { type = NodeType::MatchExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitMatchExpr(*this); }
};

struct ArrayLiteralExpr : public Expression
{
    std::vector<std::shared_ptr<AstNode>> elements;

    ArrayLiteralExpr() { type = NodeType::ArrayLiteralExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitArrayLiteralExpr(*this); }
};

struct IndexExpr : public Expression
{
    std::shared_ptr<AstNode> object;
    std::shared_ptr<AstNode> index;

    IndexExpr() { type = NodeType::IndexExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitIndexExpr(*this); }
};

struct IndexAssignExpr : public Expression
{
    std::shared_ptr<AstNode> object;
    std::shared_ptr<AstNode> index;
    std::shared_ptr<AstNode> value;

    IndexAssignExpr() { type = NodeType::IndexAssignExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitIndexAssignExpr(*this); }
};

struct MethodCallExpr : public Expression
{
    std::shared_ptr<AstNode> object;
    std::string methodName;
    std::vector<std::shared_ptr<AstNode>> arguments;

    MethodCallExpr() { type = NodeType::MethodCallExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitMethodCallExpr(*this); }
};

struct MemberAssignExpr : public Expression
{
    std::shared_ptr<AstNode> object;
    std::string memberName;
    std::shared_ptr<AstNode> value;

    MemberAssignExpr() { type = NodeType::MemberAssignExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitMemberAssignExpr(*this); }
};

struct SwitchArm
{
    std::vector<std::shared_ptr<AstNode>> values; // empty = default
    std::shared_ptr<AstNode> body;                 // single expression or block
    bool isDefault = false;
};

struct SwitchExpr : public Expression
{
    std::shared_ptr<AstNode> expr;
    std::vector<SwitchArm> arms;

    SwitchExpr() { type = NodeType::SwitchExpr; }

    void Accept(AstVisitor &visitor) override { visitor.VisitSwitchExpr(*this); }
};

} // namespace jlang
