#include "../CodeGen.h"

#include "../../Common/Logger.h"

namespace jlang
{

void CodeGenerator::VisitCallExpr(CallExpr &node)
{
    llvm::Function *callee = m_Module->getFunction(node.callee);

    if (!callee)
    {
        JLANG_ERROR(STR("Unknown function: %s", node.callee.c_str()));
    }

    std::vector<llvm::Value *> args;

    for (auto &arg : node.arguments)
    {
        arg->Accept(*this);

        if (!m_LastValue)
        {
            JLANG_ERROR(STR("Invalid argument in call to %s", node.callee.c_str()));
        }
        args.push_back(m_LastValue);
    }

    m_LastValue = m_IRBuilder.CreateCall(callee, args, node.callee + "_call");
}

void CodeGenerator::VisitLiteralExpr(LiteralExpr &node)
{
    if (node.value == "NULL" || node.value == "null" || node.value == "nullptr")
    {
        m_LastValue =
            llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context)));
    }
    else if (node.value == "true")
    {
        m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 1);
    }
    else if (node.value == "false")
    {
        m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_Context), 0);
    }
    else if (node.value.front() == '"' && node.value.back() == '"')
    {
        // String literal
        std::string strValue = node.value.substr(1, node.value.size() - 2);
        m_LastValue = m_IRBuilder.CreateGlobalStringPtr(strValue, "str");
    }
    else if (node.value.front() == '\'' && node.value.back() == '\'')
    {
        // Character literal - extract the character and create an i8 constant
        char charValue = node.value[1];
        m_LastValue =
            llvm::ConstantInt::get(llvm::Type::getInt8Ty(m_Context), static_cast<uint8_t>(charValue));
    }
    else if (node.value.find('.') != std::string::npos)
    {
        // Float literal
        try
        {
            double floatValue = std::stod(node.value);
            // Default to f64 (double) for now
            m_LastValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(m_Context), floatValue);
        }
        catch (...)
        {
            JLANG_ERROR(STR("Invalid float literal: %s", node.value.c_str()));
        }
    }
    else
    {
        // Integer literal
        try
        {
            int64_t intValue = std::stoll(node.value);
            m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), intValue);
        }
        catch (...)
        {
            JLANG_ERROR(STR("Unknown literal: %s", node.value.c_str()));
        }
    }
}

void CodeGenerator::VisitVarExpr(VarExpr &node)
{
    VariableInfo *varInfo = m_symbols.LookupVariable(node.name);

    if (!varInfo)
    {
        JLANG_ERROR(STR("Undefined variable: %s", node.name.c_str()));
        return;
    }

    // Mark variable as used
    varInfo->used = true;

    llvm::Value *storedValue = varInfo->value;

    if (llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(storedValue))
    {
        m_LastValue = m_IRBuilder.CreateLoad(alloca->getAllocatedType(), alloca, node.name);
    }
    else
    {
        m_LastValue = storedValue;
    }
}

} // namespace jlang
