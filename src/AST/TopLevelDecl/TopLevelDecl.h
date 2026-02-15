#pragma once

#include "../Ast.h"
#include "../TypeRef.h"

namespace jlang
{

struct Parameter
{
    std::string name;
    TypeRef type;
};

struct InterfaceMethodDecl
{
    std::string name;
    std::vector<Parameter> params; // excludes implicit self
    TypeRef returnType;
};

struct InterfaceDecl : public AstNode
{
    std::string name;
    std::vector<InterfaceMethodDecl> methods;

    InterfaceDecl() { type = NodeType::InterfaceDecl; }

    void Accept(AstVisitor &visitor) override { visitor.VisitInterfaceDecl(*this); }
};

struct StructField
{
    std::string name;
    TypeRef type;
    bool isPublic = false; // lowercase = private, Uppercase = public
};

struct StructDecl : public AstNode
{
    std::string name;
    std::string interfaceImplemented;
    std::vector<StructField> fields;
    std::vector<std::string> typeParameters;

    StructDecl() { type = NodeType::StructDecl; }

    void Accept(AstVisitor &visitor) override { visitor.VisitStructDecl(*this); }
};

struct FunctionDecl : public AstNode
{
    std::string name;
    std::vector<Parameter> params;
    TypeRef returnType;
    std::shared_ptr<AstNode> body;
    std::vector<std::string> typeParameters;

    FunctionDecl() { type = NodeType::FunctionDecl; }

    void Accept(AstVisitor &visitor) override { visitor.VisitFunctionDecl(*this); }
};

struct VariableDecl : public AstNode
{
    std::string name;
    TypeRef varType;
    std::shared_ptr<AstNode> initializer;
    bool isMutable = true;

    VariableDecl() { type = NodeType::VariableDecl; }

    void Accept(AstVisitor &visitor) override { visitor.VisitVariableDecl(*this); }
};

} // namespace jlang
