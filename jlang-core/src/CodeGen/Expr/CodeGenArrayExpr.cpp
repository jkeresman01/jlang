#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

namespace jlang
{

void CodeGenerator::VisitArrayLiteralExpr(ArrayLiteralExpr &node)
{
    if (node.elements.empty())
    {
        JLANG_ERROR("Array literal must have at least one element");
        return;
    }

    // Evaluate first element to determine element type
    node.elements[0]->Accept(*this);
    llvm::Value *firstVal = m_LastValue;
    if (!firstVal)
    {
        JLANG_ERROR("Invalid element in array literal");
        return;
    }

    llvm::Type *elementType = firstVal->getType();
    int arraySize = static_cast<int>(node.elements.size());
    llvm::ArrayType *arrayType = llvm::ArrayType::get(elementType, arraySize);

    // Allocate the array on the stack
    llvm::AllocaInst *alloca = m_IRBuilder.CreateAlloca(arrayType, nullptr, "arr_literal");

    // Store first element
    llvm::Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), 0);
    llvm::Value *idx0 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), 0);
    llvm::Value *gep = m_IRBuilder.CreateGEP(arrayType, alloca, {zero, idx0}, "arr_elem_0");
    m_IRBuilder.CreateStore(firstVal, gep);

    // Store remaining elements
    for (size_t i = 1; i < node.elements.size(); ++i)
    {
        node.elements[i]->Accept(*this);
        llvm::Value *val = m_LastValue;
        if (!val)
        {
            JLANG_ERROR("Invalid element in array literal");
            return;
        }

        llvm::Value *idxVal = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), i);
        llvm::Value *elemPtr =
            m_IRBuilder.CreateGEP(arrayType, alloca, {zero, idxVal}, "arr_elem_" + std::to_string(i));
        m_IRBuilder.CreateStore(val, elemPtr);
    }

    m_LastValue = alloca;
}

void CodeGenerator::VisitIndexExpr(IndexExpr &node)
{
    // Evaluate the index
    node.index->Accept(*this);
    llvm::Value *indexVal = m_LastValue;
    if (!indexVal)
    {
        JLANG_ERROR("Invalid index expression");
        return;
    }

    // Get the array/pointer base
    // We need the alloca (not the loaded value) for arrays
    std::string varName;
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.object.get()))
    {
        varName = varExpr->name;
    }

    if (!varName.empty())
    {
        VariableInfo *varInfo = m_symbols.LookupVariable(varName);
        if (!varInfo)
        {
            JLANG_ERROR(STR("Undefined variable: %s", varName.c_str()));
            return;
        }

        varInfo->used = true;
        llvm::Value *storedValue = varInfo->value;

        if (llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(storedValue))
        {
            llvm::Type *allocatedType = alloca->getAllocatedType();

            if (allocatedType->isArrayTy())
            {
                // Stack array: GEP into the array directly
                llvm::Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), 0);
                llvm::Value *elemPtr =
                    m_IRBuilder.CreateGEP(allocatedType, alloca, {zero, indexVal}, "arr_idx");
                llvm::Type *elemType = allocatedType->getArrayElementType();
                m_LastValue = m_IRBuilder.CreateLoad(elemType, elemPtr, "arr_load");
                return;
            }
            else if (allocatedType->isPointerTy())
            {
                // Heap-allocated array (pointer): load the pointer, then GEP
                llvm::Value *ptr = m_IRBuilder.CreateLoad(allocatedType, alloca, "ptr_load");

                // Determine element type from the TypeRef
                llvm::Type *elemType = nullptr;
                if (varInfo->type.isArrayType())
                {
                    TypeRef elemRef;
                    elemRef.name = varInfo->type.name;
                    elemType = MapType(elemRef);
                }
                else
                {
                    // Plain pointer - use the base type
                    TypeRef elemRef;
                    elemRef.name = varInfo->type.name;
                    elemType = MapType(elemRef);
                }

                llvm::Value *elemPtr = m_IRBuilder.CreateGEP(elemType, ptr, indexVal, "ptr_idx");
                m_LastValue = m_IRBuilder.CreateLoad(elemType, elemPtr, "ptr_load");
                return;
            }
        }
    }

    // Fallback: evaluate object normally (e.g., for nested index expressions)
    node.object->Accept(*this);
    llvm::Value *objectVal = m_LastValue;
    if (!objectVal)
    {
        JLANG_ERROR("Invalid object in index expression");
        return;
    }

    // If the object is a pointer, GEP into it
    if (objectVal->getType()->isPointerTy())
    {
        // Try to infer element type
        llvm::Type *elemType = llvm::Type::getInt32Ty(m_Context); // fallback
        llvm::Value *elemPtr = m_IRBuilder.CreateGEP(elemType, objectVal, indexVal, "idx");
        m_LastValue = m_IRBuilder.CreateLoad(elemType, elemPtr, "idx_load");
    }
    else
    {
        JLANG_ERROR("Cannot index into non-array, non-pointer type");
    }
}

void CodeGenerator::VisitIndexAssignExpr(IndexAssignExpr &node)
{
    // Evaluate the value to store
    node.value->Accept(*this);
    llvm::Value *valueToStore = m_LastValue;
    if (!valueToStore)
    {
        JLANG_ERROR("Invalid value in index assignment");
        return;
    }

    // Evaluate the index
    node.index->Accept(*this);
    llvm::Value *indexVal = m_LastValue;
    if (!indexVal)
    {
        JLANG_ERROR("Invalid index in index assignment");
        return;
    }

    // Get the array/pointer base
    std::string varName;
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.object.get()))
    {
        varName = varExpr->name;
    }

    if (!varName.empty())
    {
        VariableInfo *varInfo = m_symbols.LookupVariable(varName);
        if (!varInfo)
        {
            JLANG_ERROR(STR("Undefined variable: %s", varName.c_str()));
            return;
        }

        if (!varInfo->isMutable)
        {
            JLANG_ERROR(STR("Cannot assign to element of immutable array '%s' (declared with 'val')",
                            varName.c_str()));
            return;
        }

        varInfo->used = true;
        llvm::Value *storedValue = varInfo->value;

        if (llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(storedValue))
        {
            llvm::Type *allocatedType = alloca->getAllocatedType();

            if (allocatedType->isArrayTy())
            {
                // Stack array: GEP into the array directly
                llvm::Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_Context), 0);
                llvm::Value *elemPtr =
                    m_IRBuilder.CreateGEP(allocatedType, alloca, {zero, indexVal}, "arr_idx_assign");
                m_IRBuilder.CreateStore(valueToStore, elemPtr);
                m_LastValue = valueToStore;
                return;
            }
            else if (allocatedType->isPointerTy())
            {
                // Heap-allocated array (pointer): load the pointer, then GEP
                llvm::Value *ptr = m_IRBuilder.CreateLoad(allocatedType, alloca, "ptr_load");

                llvm::Type *elemType = nullptr;
                if (varInfo->type.isArrayType())
                {
                    TypeRef elemRef;
                    elemRef.name = varInfo->type.name;
                    elemType = MapType(elemRef);
                }
                else
                {
                    TypeRef elemRef;
                    elemRef.name = varInfo->type.name;
                    elemType = MapType(elemRef);
                }

                llvm::Value *elemPtr = m_IRBuilder.CreateGEP(elemType, ptr, indexVal, "ptr_idx_assign");
                m_IRBuilder.CreateStore(valueToStore, elemPtr);
                m_LastValue = valueToStore;
                return;
            }
        }
    }

    JLANG_ERROR("Invalid target for index assignment");
}

} // namespace jlang
