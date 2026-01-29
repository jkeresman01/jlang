#include "CodeGen.h"

#include <llvm/IR/Verifier.h>

namespace jlang
{

CodeGenerator::CodeGenerator()
    : m_Module(std::make_unique<llvm::Module>("JlangModule", m_Context)), m_IRBuilder(m_Context)
{
}

void CodeGenerator::Generate(const std::vector<std::shared_ptr<AstNode>> &program)
{
    DeclareExternalFunctions();

    for (auto &node : program)
    {
        if (node)
        {
            node->Accept(*this);
        }
    }
}

void CodeGenerator::DeclareExternalFunctions()
{
    // Declare printf: int printf(const char*, ...)
    llvm::Type *ptrType = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context));
    llvm::FunctionType *printfType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(m_Context), {ptrType}, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", m_Module.get());

    // Declare malloc: void* malloc(size_t)
    llvm::FunctionType *mallocType =
        llvm::FunctionType::get(ptrType, {llvm::Type::getInt64Ty(m_Context)}, false);
    llvm::Function::Create(mallocType, llvm::Function::ExternalLinkage, "malloc", m_Module.get());

    // Declare free: void free(void*)
    llvm::FunctionType *freeType =
        llvm::FunctionType::get(llvm::Type::getVoidTy(m_Context), {ptrType}, false);
    llvm::Function::Create(freeType, llvm::Function::ExternalLinkage, "free", m_Module.get());
}

void CodeGenerator::DumpIR()
{
    m_Module->print(llvm::outs(), nullptr);
}

} // namespace jlang
