#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

#include <algorithm>
#include <unordered_map>

namespace jlang
{

llvm::StructType *CodeGenerator::GetOrCreateResultType(const TypeRef &typeRef)
{
    if (!typeRef.isResult())
    {
        return nullptr;
    }

    std::string mangledName = typeRef.getMangledName();

    // Check if we already have this Result type
    ResultTypeInfo *existing = m_symbols.LookupResultType(mangledName);
    if (existing)
    {
        return existing->llvmType;
    }

    // Get the Ok and Err types
    const TypeRef &okTypeRef = typeRef.typeParameters[0];
    const TypeRef &errTypeRef = typeRef.typeParameters[1];

    llvm::Type *okType = MapType(okTypeRef);
    llvm::Type *errType = MapType(errTypeRef);

    // Calculate sizes to determine data field size
    const llvm::DataLayout &dataLayout = m_Module->getDataLayout();
    size_t okSize = dataLayout.getTypeAllocSize(okType);
    size_t errSize = dataLayout.getTypeAllocSize(errType);
    size_t dataSize = std::max(okSize, errSize);

    // Create Result struct: { i8 tag, [dataSize x i8] data }
    llvm::Type *tagType = llvm::Type::getInt8Ty(m_Context);
    llvm::Type *dataType = llvm::ArrayType::get(llvm::Type::getInt8Ty(m_Context), dataSize);

    llvm::StructType *resultType = llvm::StructType::create(m_Context, {tagType, dataType}, mangledName);

    // Store type info
    ResultTypeInfo info;
    info.llvmType = resultType;
    info.okType = okTypeRef;
    info.errType = errTypeRef;
    info.dataSize = dataSize;

    m_symbols.DefineResultType(mangledName, info);

    return resultType;
}

llvm::Type *CodeGenerator::MapType(const TypeRef &typeRef)
{
    // Check if this type name is a type parameter being substituted
    auto subst = m_typeSubstitutions.find(typeRef.name);
    if (subst != m_typeSubstitutions.end())
    {
        TypeRef resolved = subst->second;
        resolved.isPointer = typeRef.isPointer || resolved.isPointer;
        resolved.isNullable = typeRef.isNullable;
        resolved.isArray = typeRef.isArray;
        resolved.arraySize = typeRef.arraySize;
        return MapType(resolved);
    }

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

    // Handle Result<T, E> types
    if (typeRef.isResult())
    {
        llvm::Type *resultType = GetOrCreateResultType(typeRef);
        if (typeRef.isPointer)
        {
            return llvm::PointerType::getUnqual(resultType);
        }
        return resultType;
    }

    llvm::Type *baseType = nullptr;
    auto it = typeMap.find(typeRef.name);
    if (it != typeMap.end())
    {
        baseType = it->second(m_Context);
    }
    else
    {
        // Check if it's a registered interface type
        InterfaceInfo *ifaceInfo = m_symbols.LookupInterface(typeRef.name);
        if (ifaceInfo)
        {
            // Interface variables are fat pointers
            if (typeRef.isPointer)
            {
                return llvm::PointerType::getUnqual(ifaceInfo->fatPtrType);
            }
            return ifaceInfo->fatPtrType;
        }

        // Check if this is a generic struct that needs instantiation
        if (typeRef.isGeneric() && !typeRef.isResult())
        {
            std::string mangledName = typeRef.getMangledName();
            StructInfo *si = m_symbols.LookupStruct(mangledName);
            if (!si)
            {
                StructDecl *templ = m_symbols.LookupGenericStruct(typeRef.name);
                if (templ)
                {
                    InstantiateGenericStruct(*templ, typeRef.typeParameters);
                    si = m_symbols.LookupStruct(mangledName);
                }
            }
            if (si)
            {
                baseType = si->llvmType;
            }
            else
            {
                baseType = llvm::Type::getInt8Ty(m_Context);
            }
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
    }

    // Handle array types: e.g. i32[5] -> [5 x i32]
    if (typeRef.isArrayType())
    {
        llvm::Type *arrayType = llvm::ArrayType::get(baseType, typeRef.arraySize);
        if (typeRef.isPointer)
        {
            return llvm::PointerType::getUnqual(baseType);
        }
        return arrayType;
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

    // Handle array types
    if (auto *arrayType = llvm::dyn_cast<llvm::ArrayType>(llvmType))
    {
        TypeRef elemRef = InferTypeRef(arrayType->getElementType());
        elemRef.isArray = true;
        elemRef.arraySize = static_cast<int>(arrayType->getNumElements());
        return elemRef;
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
