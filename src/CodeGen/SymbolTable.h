#pragma once

#include "../AST/TypeRef.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace jlang
{

struct FieldInfo
{
    unsigned index;
    TypeRef type;
    bool isPublic;
};

struct StructInfo
{
    llvm::StructType *llvmType;
    std::unordered_map<std::string, FieldInfo> fields;
};

struct VariableInfo
{
    llvm::Value *value;
    TypeRef type;
    bool used = false;
    bool isMutable = true;
    bool isResultMatched = false; // For Result type safety tracking
};

struct ResultTypeInfo
{
    llvm::StructType *llvmType;
    TypeRef okType;
    TypeRef errType;
    size_t dataSize; // Size of the data field (max of ok and err types)
};

class SymbolTable
{
  public:
    void DefineVariable(const std::string &name, const VariableInfo &info) { m_variables[name] = info; }

    VariableInfo *LookupVariable(const std::string &name)
    {
        auto it = m_variables.find(name);
        return it != m_variables.end() ? &it->second : nullptr;
    }

    void EraseVariable(const std::string &name) { m_variables.erase(name); }

    void DefineStruct(const std::string &name, const StructInfo &info) { m_structTypes[name] = info; }

    StructInfo *LookupStruct(const std::string &name)
    {
        auto it = m_structTypes.find(name);
        return it != m_structTypes.end() ? &it->second : nullptr;
    }

    void EnterFunctionScope() { m_currentFunctionVariables.clear(); }

    void LeaveFunctionScope()
    {
        for (const auto &varName : m_currentFunctionVariables)
        {
            m_variables.erase(varName);
        }
        m_currentFunctionVariables.clear();
    }

    void TrackFunctionLocal(const std::string &name) { m_currentFunctionVariables.insert(name); }

    const std::unordered_set<std::string> &GetCurrentFunctionVariables() const
    {
        return m_currentFunctionVariables;
    }

    void DefineResultType(const std::string &mangledName, const ResultTypeInfo &info)
    {
        m_resultTypes[mangledName] = info;
    }

    ResultTypeInfo *LookupResultType(const std::string &mangledName)
    {
        auto it = m_resultTypes.find(mangledName);
        return it != m_resultTypes.end() ? &it->second : nullptr;
    }

  private:
    std::unordered_map<std::string, VariableInfo> m_variables;
    std::unordered_map<std::string, StructInfo> m_structTypes;
    std::unordered_set<std::string> m_currentFunctionVariables;
    std::unordered_map<std::string, ResultTypeInfo> m_resultTypes;
};

} // namespace jlang
