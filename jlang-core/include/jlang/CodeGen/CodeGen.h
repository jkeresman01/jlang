#pragma once

#include <jlang/CodeGen/AstVisitor.h>
#include <jlang/CodeGen/SymbolTable.h>

#include <jlang/AST/Ast.h>
#include <jlang/AST/Expressions/Expressions.h>
#include <jlang/AST/Statements/Statements.h>
#include <jlang/AST/TopLevelDecl/TopLevelDecl.h>

#include <memory>
#include <unordered_map>

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace jlang
{

class CodeGenerator : public AstVisitor
{
  public:
    CodeGenerator();

    void Generate(const std::vector<std::shared_ptr<AstNode>> &program);
    void DumpIR();
    bool EmitExecutable(const std::string &outputPath);

  private:
    virtual void VisitFunctionDecl(FunctionDecl &) override;
    virtual void VisitInterfaceDecl(InterfaceDecl &) override;
    virtual void VisitStructDecl(StructDecl &) override;
    virtual void VisitVariableDecl(VariableDecl &) override;

    virtual void VisitIfStatement(IfStatement &) override;
    virtual void VisitWhileStatement(WhileStatement &) override;
    virtual void VisitForStatement(ForStatement &) override;
    virtual void VisitForEachStatement(ForEachStatement &) override;
    virtual void VisitBlockStatement(BlockStatement &) override;
    virtual void VisitExprStatement(ExprStatement &) override;
    virtual void VisitReturnStatement(ReturnStatement &) override;
    virtual void VisitBreakStatement(BreakStatement &) override;
    virtual void VisitContinueStatement(ContinueStatement &) override;
    virtual void VisitSwitchStatement(SwitchStatement &) override;

    virtual void VisitCallExpr(CallExpr &) override;
    virtual void VisitBinaryExpr(BinaryExpr &) override;
    virtual void VisitUnaryExpr(UnaryExpr &) override;
    virtual void VisitLiteralExpr(LiteralExpr &) override;
    virtual void VisitVarExpr(VarExpr &) override;
    virtual void VisitCastExpr(CastExpr &) override;
    virtual void VisitAllocExpr(AllocExpr &) override;
    virtual void VisitAssignExpr(AssignExpr &) override;
    virtual void VisitMemberAccessExpr(MemberAccessExpr &) override;
    virtual void VisitPrefixExpr(PrefixExpr &) override;
    virtual void VisitPostfixExpr(PostfixExpr &) override;
    virtual void VisitMatchExpr(MatchExpr &) override;
    virtual void VisitOkExpr(OkExpr &) override;
    virtual void VisitErrExpr(ErrExpr &) override;
    virtual void VisitArrayLiteralExpr(ArrayLiteralExpr &) override;
    virtual void VisitIndexExpr(IndexExpr &) override;
    virtual void VisitIndexAssignExpr(IndexAssignExpr &) override;
    virtual void VisitMethodCallExpr(MethodCallExpr &) override;
    virtual void VisitMemberAssignExpr(MemberAssignExpr &) override;
    virtual void VisitSwitchExpr(SwitchExpr &) override;

  private:
    void DeclareExternalFunctions();
    llvm::Type *MapType(const TypeRef &typeRef);
    TypeRef InferTypeRef(llvm::Type *llvmType);
    llvm::StructType *GetOrCreateResultType(const TypeRef &typeRef);
    llvm::StructType *GetOrCreateVectorType(const TypeRef &typeRef);
    void GenerateVtables();
    std::string DetermineStructTypeName(AstNode *node);

    void InstantiateGenericFunction(FunctionDecl &templ, const std::vector<TypeRef> &typeArgs);
    void InstantiateGenericStruct(StructDecl &templ, const std::vector<TypeRef> &typeArgs);
    std::string MangleGenericName(const std::string &baseName, const std::vector<TypeRef> &typeArgs);

  private:
    llvm::LLVMContext m_Context;
    std::unique_ptr<llvm::Module> m_Module;
    llvm::IRBuilder<> m_IRBuilder;

    SymbolTable m_symbols;
    llvm::Value *m_LastValue = nullptr;
    std::vector<llvm::BasicBlock *> m_LoopExitStack;
    std::vector<llvm::BasicBlock *> m_LoopContinueStack;
    std::unordered_map<std::string, TypeRef> m_typeSubstitutions;
};

} // namespace jlang
