#include "CodeGen.h"

#include <unordered_map>

namespace jlang
{

llvm::Type *CodeGenerator::MapType(const TypeRef &typeRef)
{
    using TypeGetter = llvm::Type *(*)(llvm::LLVMContext &);

    static const std::unordered_map<std::string, TypeGetter> typeMap = {
        {"i8", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt8Ty(ctx); }},
        {"u8", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt8Ty(ctx); }},
        {"char", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt8Ty(ctx); }},
        {"i16", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt16Ty(ctx); }},
        {"u16", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt16Ty(ctx); }},
        {"i32", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt32Ty(ctx); }},
        {"u32", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt32Ty(ctx); }},
        {"i64", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt64Ty(ctx); }},
        {"u64", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt64Ty(ctx); }},
        {"f32", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getFloatTy(ctx); }},
        {"f64", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getDoubleTy(ctx); }},
        {"bool", +[](llvm::LLVMContext &ctx) -> llvm::Type * { return llvm::Type::getInt1Ty(ctx); }},
    };

    if (typeRef.name == "void")
    {
        return llvm::Type::getVoidTy(m_Context);
    }

    llvm::Type *baseType = nullptr;
    auto it = typeMap.find(typeRef.name);
    if (it != typeMap.end())
    {
        baseType = it->second(m_Context);
    }
    else
    {
        // Check if it's a registered struct type
        StructInfo *structInfo = m_symbols.LookupStruct(typeRef.name);
        if (structInfo)
        {
            baseType = structInfo->llvmType;
        }
        else
        {
            // Unknown user-defined type - use i8 as placeholder
            baseType = llvm::Type::getInt8Ty(m_Context);
        }
    }

    if (typeRef.isPointer)
    {
        return llvm::PointerType::getUnqual(baseType);
    }

    return baseType;
}

TypeRef CodeGenerator::InferTypeRef(llvm::Type *llvmType)
{
    // Handle pointer types
    if (llvmType->isPointerTy())
    {
        // In opaque pointer mode (LLVM 15+), we can't get the element type
        // Default to i8* (char*) for generic pointers
        return TypeRef{"char", true};
    }

    // Handle integer types
    if (llvmType->isIntegerTy())
    {
        unsigned bitWidth = llvmType->getIntegerBitWidth();
        switch (bitWidth)
        {
        case 1:
            return TypeRef{"bool", false};
        case 8:
            return TypeRef{"i8", false};
        case 16:
            return TypeRef{"i16", false};
        case 32:
            return TypeRef{"i32", false};
        case 64:
            return TypeRef{"i64", false};
        default:
            return TypeRef{"i32", false}; // Default to i32
        }
    }

    // Handle floating point types
    if (llvmType->isFloatTy())
    {
        return TypeRef{"f32", false};
    }
    if (llvmType->isDoubleTy())
    {
        return TypeRef{"f64", false};
    }

    // Handle struct types
    if (auto *structType = llvm::dyn_cast<llvm::StructType>(llvmType))
    {
        if (structType->hasName())
        {
            return TypeRef{structType->getName().str(), false};
        }
    }

    // Default fallback
    return TypeRef{"i32", false};
}

} // namespace jlang
