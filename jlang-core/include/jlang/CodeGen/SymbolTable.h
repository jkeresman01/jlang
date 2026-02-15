#pragma once

#include <jlang/AST/TypeRef.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace jlang
{

struct FunctionDecl;
struct StructDecl;

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
    std::string interfaceImplemented;
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

struct VectorTypeInfo
{
    llvm::StructType *llvmType;
    TypeRef elementType;
};

struct InterfaceMethodInfo
{
    std::string name;
    std::vector<TypeRef> paramTypes; // excludes self
    TypeRef returnType;
    unsigned vtableIndex;
    llvm::FunctionType *llvmFuncType; // with i8* as self
};

struct InterfaceInfo
{
    std::string name;
    std::vector<InterfaceMethodInfo> methods;
    llvm::StructType *vtableType;
    llvm::StructType *fatPtrType;
};

struct StructInterfaceInfo
{
    std::string interfaceName;
    llvm::GlobalVariable *vtableGlobal;
};

using VariableMap = std::unordered_map<std::string, VariableInfo>;
using StructMap = std::unordered_map<std::string, StructInfo>;
using ResultTypeMap = std::unordered_map<std::string, ResultTypeInfo>;
using VectorTypeMap = std::unordered_map<std::string, VectorTypeInfo>;
using InterfaceMap = std::unordered_map<std::string, InterfaceInfo>;
using StructInterfaceMap = std::unordered_map<std::string, StructInterfaceInfo>;
using GenericFunctionMap = std::unordered_map<std::string, FunctionDecl *>;
using GenericStructMap = std::unordered_map<std::string, StructDecl *>;
using NameSet = std::unordered_set<std::string>;

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

    const NameSet &GetCurrentFunctionVariables() const { return m_currentFunctionVariables; }

    void DefineResultType(const std::string &mangledName, const ResultTypeInfo &info)
    {
        m_resultTypes[mangledName] = info;
    }

    ResultTypeInfo *LookupResultType(const std::string &mangledName)
    {
        auto it = m_resultTypes.find(mangledName);
        return it != m_resultTypes.end() ? &it->second : nullptr;
    }

    void DefineVectorType(const std::string &mangledName, const VectorTypeInfo &info)
    {
        m_vectorTypes[mangledName] = info;
    }

    VectorTypeInfo *LookupVectorType(const std::string &mangledName)
    {
        auto it = m_vectorTypes.find(mangledName);
        return it != m_vectorTypes.end() ? &it->second : nullptr;
    }

    void DefineInterface(const std::string &name, const InterfaceInfo &info) { m_interfaces[name] = info; }

    InterfaceInfo *LookupInterface(const std::string &name)
    {
        auto it = m_interfaces.find(name);
        return it != m_interfaces.end() ? &it->second : nullptr;
    }

    void DefineStructInterface(const std::string &structName, const StructInterfaceInfo &info)
    {
        m_structInterfaces[structName] = info;
    }

    StructInterfaceInfo *LookupStructInterface(const std::string &structName)
    {
        auto it = m_structInterfaces.find(structName);
        return it != m_structInterfaces.end() ? &it->second : nullptr;
    }

    const StructInterfaceMap &GetAllStructInterfaces() const { return m_structInterfaces; }

    const InterfaceMap &GetAllInterfaces() const { return m_interfaces; }

    void DefineGenericFunction(const std::string &name, FunctionDecl *decl)
    {
        m_genericFunctions[name] = decl;
    }

    FunctionDecl *LookupGenericFunction(const std::string &name)
    {
        auto it = m_genericFunctions.find(name);
        return it != m_genericFunctions.end() ? it->second : nullptr;
    }

    void DefineGenericStruct(const std::string &name, StructDecl *decl) { m_genericStructs[name] = decl; }

    StructDecl *LookupGenericStruct(const std::string &name)
    {
        auto it = m_genericStructs.find(name);
        return it != m_genericStructs.end() ? it->second : nullptr;
    }

    bool IsInstantiated(const std::string &mangledName)
    {
        return m_instantiatedGenerics.count(mangledName) > 0;
    }

    void MarkInstantiated(const std::string &mangledName) { m_instantiatedGenerics.insert(mangledName); }

  private:
    VariableMap m_variables;
    StructMap m_structTypes;
    NameSet m_currentFunctionVariables;
    ResultTypeMap m_resultTypes;
    VectorTypeMap m_vectorTypes;
    InterfaceMap m_interfaces;
    StructInterfaceMap m_structInterfaces;
    GenericFunctionMap m_genericFunctions;
    GenericStructMap m_genericStructs;
    NameSet m_instantiatedGenerics;
};

} // namespace jlang
