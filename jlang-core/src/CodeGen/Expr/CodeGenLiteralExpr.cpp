#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

#include <climits>

namespace jlang
{

void CodeGenerator::VisitCallExpr(CallExpr &node)
{
    // Handle std::Vector<T>() constructor
    if (node.callee == "std::Vector" && !node.typeArguments.empty())
    {
        TypeRef vecTypeRef;
        vecTypeRef.name = "std::Vector";
        vecTypeRef.typeParameters = node.typeArguments;

        llvm::StructType *vecStructType = GetOrCreateVectorType(vecTypeRef);
        if (!vecStructType)
        {
            JLANG_ERROR("Failed to create vector type");
            return;
        }

        const TypeRef &elemTypeRef = node.typeArguments[0];
        llvm::Type *elemType = MapType(elemTypeRef);
        const llvm::DataLayout &dataLayout = m_Module->getDataLayout();
        uint64_t elemSize = dataLayout.getTypeAllocSize(elemType);

        // Determine initial capacity
        int64_t initialCap = 8;
        if (!node.arguments.empty())
        {
            node.arguments[0]->Accept(*this);
            if (auto *constInt = llvm::dyn_cast<llvm::ConstantInt>(m_LastValue))
            {
                initialCap = constInt->getSExtValue();
            }
        }

        // Allocate the vector struct on the stack
        llvm::AllocaInst *vecAlloca = m_IRBuilder.CreateAlloca(vecStructType, nullptr, "vec");

        // malloc initial buffer: capacity * elem_size
        llvm::Function *mallocFunc = m_Module->getFunction("malloc");
        llvm::Value *bufSize = llvm::ConstantInt::get(llvm::Type::getInt64Ty(m_Context),
                                                      initialCap * static_cast<int64_t>(elemSize));
        llvm::Value *dataBuf = m_IRBuilder.CreateCall(mallocFunc, {bufSize}, "vec_data");

        // Cast to T*
        llvm::Type *elemPtrType = llvm::PointerType::getUnqual(elemType);
        llvm::Value *typedData = m_IRBuilder.CreateBitCast(dataBuf, elemPtrType, "vec_data_typed");

        // Store data pointer (index 0)
        llvm::Value *dataField = m_IRBuilder.CreateStructGEP(vecStructType, vecAlloca, 0, "vec_data_ptr");
        m_IRBuilder.CreateStore(typedData, dataField);

        // Store size = 0 (index 1)
        llvm::Value *sizeField = m_IRBuilder.CreateStructGEP(vecStructType, vecAlloca, 1, "vec_size_ptr");
        m_IRBuilder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(m_Context), 0), sizeField);

        // Store capacity (index 2)
        llvm::Value *capField = m_IRBuilder.CreateStructGEP(vecStructType, vecAlloca, 2, "vec_cap_ptr");
        m_IRBuilder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(m_Context), initialCap),
                                capField);

        m_LastValue = vecAlloca;
        return;
    }

    // Handle generic function calls: instantiate if needed
    if (!node.typeArguments.empty())
    {
        FunctionDecl *templ = m_symbols.LookupGenericFunction(node.callee);
        if (templ)
        {
            std::string mangledName = MangleGenericName(node.callee, node.typeArguments);
            if (!m_symbols.IsInstantiated(mangledName))
            {
                InstantiateGenericFunction(*templ, node.typeArguments);
            }
            // Redirect callee to mangled name
            node.callee = mangledName;
        }
    }

    llvm::Function *callee = m_Module->getFunction(node.callee);

    // If not found, try mangled name based on first argument type
    if (!callee && !node.arguments.empty())
    {
        // Peek at first arg to determine struct type
        if (auto *varExpr = dynamic_cast<VarExpr *>(node.arguments[0].get()))
        {
            VariableInfo *argInfo = m_symbols.LookupVariable(varExpr->name);
            if (argInfo)
            {
                std::string mangledName = argInfo->type.name + "_" + node.callee;
                callee = m_Module->getFunction(mangledName);
            }
        }
    }

    if (!callee)
    {
        JLANG_ERROR(STR("Unknown function: %s", node.callee.c_str()));
        return;
    }

    std::vector<llvm::Value *> args;

    for (size_t i = 0; i < node.arguments.size(); ++i)
    {
        node.arguments[i]->Accept(*this);

        if (!m_LastValue)
        {
            JLANG_ERROR(STR("Invalid argument in call to %s", node.callee.c_str()));
        }

        // Check if we need implicit struct*-to-interface conversion for this argument
        if (callee && i < callee->arg_size())
        {
            llvm::Type *expectedType = callee->getArg(i)->getType();

            // If the expected type is an interface fat pointer struct, and we're passing a struct pointer
            if (expectedType->isStructTy() && m_LastValue->getType()->isPointerTy())
            {
                llvm::StructType *expectedStruct = llvm::dyn_cast<llvm::StructType>(expectedType);
                if (expectedStruct && expectedStruct->hasName())
                {
                    std::string expectedName = expectedStruct->getName().str();
                    // Check if this is a fat_ptr type
                    if (expectedName.find("_fat_ptr") != std::string::npos)
                    {
                        // Extract interface name from "InterfaceName_fat_ptr"
                        std::string ifaceName = expectedName.substr(0, expectedName.find("_fat_ptr"));
                        InterfaceInfo *ifaceInfo = m_symbols.LookupInterface(ifaceName);

                        if (ifaceInfo)
                        {
                            // Determine struct type of the argument
                            std::string argStructName;
                            if (auto *argVar = dynamic_cast<VarExpr *>(node.arguments[i].get()))
                            {
                                VariableInfo *ai = m_symbols.LookupVariable(argVar->name);
                                if (ai)
                                    argStructName = ai->type.name;
                            }

                            if (!argStructName.empty())
                            {
                                StructInterfaceInfo *siInfo = m_symbols.LookupStructInterface(argStructName);
                                if (siInfo && siInfo->vtableGlobal)
                                {
                                    // Create fat pointer on the fly
                                    llvm::Type *i8Ptr =
                                        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m_Context));
                                    llvm::Value *dataPtr =
                                        m_IRBuilder.CreateBitCast(m_LastValue, i8Ptr, "arg_data");

                                    llvm::AllocaInst *fatPtr =
                                        m_IRBuilder.CreateAlloca(ifaceInfo->fatPtrType, nullptr, "arg_fat");

                                    llvm::Value *dataField = m_IRBuilder.CreateStructGEP(
                                        ifaceInfo->fatPtrType, fatPtr, 0, "arg_fat_data");
                                    m_IRBuilder.CreateStore(dataPtr, dataField);

                                    llvm::Value *vtableField = m_IRBuilder.CreateStructGEP(
                                        ifaceInfo->fatPtrType, fatPtr, 1, "arg_fat_vtable");
                                    m_IRBuilder.CreateStore(siInfo->vtableGlobal, vtableField);

                                    // Load the fat pointer struct to pass by value
                                    m_LastValue =
                                        m_IRBuilder.CreateLoad(ifaceInfo->fatPtrType, fatPtr, "arg_fat_val");
                                }
                            }
                        }
                    }
                }
            }
        }

        args.push_back(m_LastValue);
    }

    if (callee->getReturnType()->isVoidTy())
    {
        m_IRBuilder.CreateCall(callee, args);
        m_LastValue = nullptr;
    }
    else
    {
        m_LastValue = m_IRBuilder.CreateCall(callee, args, node.callee + "_call");
    }
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
            if (intValue > INT32_MAX || intValue < INT32_MIN)
            {
                m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt64Ty(m_Context), intValue);
            }
            else
            {
                m_LastValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), intValue);
            }
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
        // For array types, return a pointer to the first element instead of loading
        if (alloca->getAllocatedType()->isArrayTy())
        {
            llvm::Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), 0);
            m_LastValue =
                m_IRBuilder.CreateGEP(alloca->getAllocatedType(), alloca, {zero, zero}, node.name + "_ptr");
        }
        else
        {
            m_LastValue = m_IRBuilder.CreateLoad(alloca->getAllocatedType(), alloca, node.name);
        }
    }
    else
    {
        m_LastValue = storedValue;
    }
}

} // namespace jlang
