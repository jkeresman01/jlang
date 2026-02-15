#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

#include <algorithm>
#include <cstdlib>

#include <llvm/IR/Constants.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

namespace jlang
{

CodeGenerator::CodeGenerator()
    : m_Module(std::make_unique<llvm::Module>("JlangModule", m_Context)), m_IRBuilder(m_Context)
{
}

void CodeGenerator::Generate(const std::vector<std::shared_ptr<AstNode>> &program)
{
    DeclareExternalFunctions();

    // Pass 0: Collect generic templates into the symbol table (skip concrete codegen for them)
    for (auto &node : program)
    {
        if (auto *fn = dynamic_cast<FunctionDecl *>(node.get()))
        {
            if (!fn->typeParameters.empty())
                m_symbols.DefineGenericFunction(fn->name, fn);
        }
        if (auto *st = dynamic_cast<StructDecl *>(node.get()))
        {
            if (!st->typeParameters.empty())
                m_symbols.DefineGenericStruct(st->name, st);
        }
    }

    // Pass 1: Visit interface and struct declarations (builds types) — skip generic structs
    for (auto &node : program)
    {
        if (node && (node->type == NodeType::InterfaceDecl || node->type == NodeType::StructDecl))
        {
            if (node->type == NodeType::StructDecl)
            {
                auto &structNode = static_cast<StructDecl &>(*node);
                if (!structNode.typeParameters.empty())
                    continue; // Skip generic struct templates
            }
            node->Accept(*this);
        }
    }

    // Pass 2: Create function signatures only (no bodies) — skip generic functions
    for (auto &node : program)
    {
        if (node && node->type == NodeType::FunctionDecl)
        {
            auto &funcNode = static_cast<FunctionDecl &>(*node);

            if (!funcNode.typeParameters.empty())
                continue; // Skip generic function templates

            m_symbols.EnterFunctionScope();

            std::vector<llvm::Type *> paramTypes;
            paramTypes.reserve(funcNode.params.size());
            std::transform(funcNode.params.begin(), funcNode.params.end(), std::back_inserter(paramTypes),
                           [this](const Parameter &param) { return MapType(param.type); });

            llvm::FunctionType *funcType =
                llvm::FunctionType::get(MapType(funcNode.returnType), paramTypes, false);

            // Name mangling
            std::string llvmName = funcNode.name;
            if (!funcNode.params.empty() && funcNode.params[0].name == "self" &&
                funcNode.params[0].type.isPointer)
            {
                StructInfo *si = m_symbols.LookupStruct(funcNode.params[0].type.name);
                if (si)
                {
                    llvmName = funcNode.params[0].type.name + "_" + funcNode.name;
                }
            }

            llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, llvmName, m_Module.get());

            m_symbols.LeaveFunctionScope();
        }
    }

    // Pass 3: Generate vtables (now all function signatures exist)
    GenerateVtables();

    // Pass 4: Visit function declarations with bodies — skip generic functions
    for (auto &node : program)
    {
        if (node && node->type == NodeType::FunctionDecl)
        {
            auto &funcNode = static_cast<FunctionDecl &>(*node);
            if (!funcNode.typeParameters.empty())
                continue; // Skip generic function templates
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

void CodeGenerator::GenerateVtables()
{
    for (auto &[structName, siInfo] : m_symbols.GetAllStructInterfaces())
    {
        InterfaceInfo *ifaceInfo = m_symbols.LookupInterface(siInfo.interfaceName);
        if (!ifaceInfo)
            continue;

        // Build vtable constant entries
        std::vector<llvm::Constant *> vtableEntries;
        for (auto &method : ifaceInfo->methods)
        {
            std::string mangledName = structName + "_" + method.name;
            llvm::Function *func = m_Module->getFunction(mangledName);
            if (!func)
            {
                JLANG_ERROR(STR("Vtable: function '%s' not found for struct '%s' implementing '%s'",
                                mangledName.c_str(), structName.c_str(), siInfo.interfaceName.c_str()));
                return;
            }

            // Bitcast the function to the vtable slot type (i8* as self instead of StructName*)
            llvm::Constant *castedFunc =
                llvm::ConstantExpr::getBitCast(func, llvm::PointerType::getUnqual(method.llvmFuncType));
            vtableEntries.push_back(castedFunc);
        }

        // Create vtable global constant
        llvm::Constant *vtableInit = llvm::ConstantStruct::get(ifaceInfo->vtableType, vtableEntries);
        std::string vtableGlobalName = structName + "_" + siInfo.interfaceName + "_vtable";
        auto *vtableGlobal =
            new llvm::GlobalVariable(*m_Module, ifaceInfo->vtableType, true,
                                     llvm::GlobalValue::InternalLinkage, vtableInit, vtableGlobalName);

        // Update the StructInterfaceInfo with the vtable global
        StructInterfaceInfo updated = siInfo;
        updated.vtableGlobal = vtableGlobal;
        m_symbols.DefineStructInterface(structName, updated);
    }
}

std::string CodeGenerator::DetermineStructTypeName(AstNode *node)
{
    if (auto *varExpr = dynamic_cast<VarExpr *>(node))
    {
        VariableInfo *info = m_symbols.LookupVariable(varExpr->name);
        if (info)
        {
            // For generic types, return mangled name (e.g. Pair_i32_f64)
            if (info->type.isGeneric() && !info->type.isResult())
            {
                TypeRef baseType = info->type;
                baseType.isPointer = false;
                baseType.isNullable = false;
                return baseType.getMangledName();
            }
            return info->type.name;
        }
    }
    return "";
}

void CodeGenerator::DumpIR()
{
    m_Module->print(llvm::outs(), nullptr);
}

std::string CodeGenerator::MangleGenericName(const std::string &baseName,
                                             const std::vector<TypeRef> &typeArgs)
{
    std::string mangled = baseName;
    for (const auto &arg : typeArgs)
    {
        mangled += "_" + arg.getMangledName();
    }
    return mangled;
}

void CodeGenerator::InstantiateGenericFunction(FunctionDecl &templ, const std::vector<TypeRef> &typeArgs)
{
    std::string mangledName = MangleGenericName(templ.name, typeArgs);

    if (m_symbols.IsInstantiated(mangledName))
        return;

    // Save current IRBuilder state (we may be in the middle of generating another function)
    llvm::BasicBlock *savedBlock = m_IRBuilder.GetInsertBlock();
    llvm::BasicBlock::iterator savedPoint = m_IRBuilder.GetInsertPoint();
    auto oldSubstitutions = m_typeSubstitutions;

    // Build substitution map: T->i32, U->char*, etc.
    for (size_t i = 0; i < templ.typeParameters.size() && i < typeArgs.size(); i++)
        m_typeSubstitutions[templ.typeParameters[i]] = typeArgs[i];

    // Create a modified FunctionDecl with mangled name and substituted types
    FunctionDecl specialized = templ; // shallow copy
    specialized.name = mangledName;
    specialized.typeParameters.clear(); // no longer generic

    // Create function signature (Pass 2 equivalent)
    m_symbols.EnterFunctionScope();

    std::vector<llvm::Type *> paramTypes;
    for (auto &param : specialized.params)
        paramTypes.push_back(MapType(param.type));

    auto *funcType = llvm::FunctionType::get(MapType(specialized.returnType), paramTypes, false);
    llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, specialized.name, m_Module.get());

    m_symbols.LeaveFunctionScope();

    // Generate function body (Pass 4 equivalent)
    VisitFunctionDecl(specialized);

    // Restore state
    m_typeSubstitutions = oldSubstitutions;
    if (savedBlock)
        m_IRBuilder.SetInsertPoint(savedBlock, savedPoint);

    m_symbols.MarkInstantiated(mangledName);
}

void CodeGenerator::InstantiateGenericStruct(StructDecl &templ, const std::vector<TypeRef> &typeArgs)
{
    std::string mangledName = MangleGenericName(templ.name, typeArgs);

    if (m_symbols.IsInstantiated(mangledName))
        return;

    auto oldSubstitutions = m_typeSubstitutions;
    for (size_t i = 0; i < templ.typeParameters.size() && i < typeArgs.size(); i++)
        m_typeSubstitutions[templ.typeParameters[i]] = typeArgs[i];

    StructDecl specialized = templ; // shallow copy
    specialized.name = mangledName;
    specialized.typeParameters.clear();

    // Visit will create the LLVM struct type via VisitStructDecl
    VisitStructDecl(specialized);

    m_typeSubstitutions = oldSubstitutions;
    m_symbols.MarkInstantiated(mangledName);
}

bool CodeGenerator::EmitExecutable(const std::string &outputPath)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    std::string targetTriple = llvm::sys::getDefaultTargetTriple();
    m_Module->setTargetTriple(targetTriple);

    std::string error;
    const llvm::Target *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (!target)
    {
        std::cerr << "Error: Failed to lookup target: " << error << "\n";
        return false;
    }

    llvm::TargetOptions opts;
    auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opts, llvm::Reloc::PIC_);
    m_Module->setDataLayout(targetMachine->createDataLayout());

    // Emit object file to a temporary path
    std::string objPath = outputPath + ".o";
    std::error_code ec;
    llvm::raw_fd_ostream dest(objPath, ec, llvm::sys::fs::OF_None);
    if (ec)
    {
        std::cerr << "Error: Could not open output file: " << ec.message() << "\n";
        return false;
    }

    llvm::legacy::PassManager pass;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile))
    {
        std::cerr << "Error: Target machine cannot emit object file\n";
        return false;
    }

    pass.run(*m_Module);
    dest.flush();

    // Link the object file into an executable using the system compiler
    std::string linkCmd = "cc " + objPath + " -o " + outputPath + " -lm";
    int result = std::system(linkCmd.c_str());
    std::remove(objPath.c_str());

    if (result != 0)
    {
        std::cerr << "Error: Linking failed (exit code " << result << ")\n";
        return false;
    }

    return true;
}

} // namespace jlang
