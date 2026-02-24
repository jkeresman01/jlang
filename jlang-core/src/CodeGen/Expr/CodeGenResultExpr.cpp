#include <jlang/CodeGen/CodeGen.h>

#include <jlang/Common/Logger.h>

namespace jlang
{

void CodeGenerator::VisitOkExpr(OkExpr &node)
{
    if (!node.resultType.isResult())
    {
        JLANG_ERROR("Ok expression requires Result type context");
        m_LastValue = nullptr;
        return;
    }

    // Get the Result struct type
    llvm::StructType *resultType = GetOrCreateResultType(node.resultType);
    if (!resultType)
    {
        JLANG_ERROR("Failed to create Result type");
        m_LastValue = nullptr;
        return;
    }

    // Evaluate the Ok value
    node.value->Accept(*this);
    llvm::Value *okValue = m_LastValue;
    if (!okValue)
    {
        JLANG_ERROR("Invalid value in Ok expression");
        return;
    }

    // Allocate Result struct
    llvm::AllocaInst *resultAlloca = m_IRBuilder.CreateAlloca(resultType, nullptr, "result_ok");

    // Set tag = 0 (Ok)
    llvm::Value *tagPtr = m_IRBuilder.CreateStructGEP(resultType, resultAlloca, 0, "tag_ptr");
    m_IRBuilder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt8Ty(m_Context), 0), tagPtr);

    // Get pointer to data field
    llvm::Value *dataPtr = m_IRBuilder.CreateStructGEP(resultType, resultAlloca, 1, "data_ptr");

    // Cast data field to the Ok type and store
    llvm::Type *okType = MapType(node.resultType.typeParameters[0]);
    llvm::Value *typedDataPtr =
        m_IRBuilder.CreateBitCast(dataPtr, llvm::PointerType::getUnqual(okType), "ok_ptr");
    m_IRBuilder.CreateStore(okValue, typedDataPtr);

    // Load the result struct as the expression value
    m_LastValue = m_IRBuilder.CreateLoad(resultType, resultAlloca, "result_val");
}

void CodeGenerator::VisitErrExpr(ErrExpr &node)
{
    if (!node.resultType.isResult())
    {
        JLANG_ERROR("Err expression requires Result type context");
        m_LastValue = nullptr;
        return;
    }

    // Get the Result struct type
    llvm::StructType *resultType = GetOrCreateResultType(node.resultType);
    if (!resultType)
    {
        JLANG_ERROR("Failed to create Result type");
        m_LastValue = nullptr;
        return;
    }

    // Evaluate the Err value
    node.error->Accept(*this);
    llvm::Value *errValue = m_LastValue;
    if (!errValue)
    {
        JLANG_ERROR("Invalid value in Err expression");
        return;
    }

    // Allocate Result struct
    llvm::AllocaInst *resultAlloca = m_IRBuilder.CreateAlloca(resultType, nullptr, "result_err");

    // Set tag = 1 (Err)
    llvm::Value *tagPtr = m_IRBuilder.CreateStructGEP(resultType, resultAlloca, 0, "tag_ptr");
    m_IRBuilder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt8Ty(m_Context), 1), tagPtr);

    // Get pointer to data field
    llvm::Value *dataPtr = m_IRBuilder.CreateStructGEP(resultType, resultAlloca, 1, "data_ptr");

    // Cast data field to the Err type and store
    llvm::Type *errType = MapType(node.resultType.typeParameters[1]);
    llvm::Value *typedDataPtr =
        m_IRBuilder.CreateBitCast(dataPtr, llvm::PointerType::getUnqual(errType), "err_ptr");
    m_IRBuilder.CreateStore(errValue, typedDataPtr);

    // Load the result struct as the expression value
    m_LastValue = m_IRBuilder.CreateLoad(resultType, resultAlloca, "result_val");
}

void CodeGenerator::VisitMatchExpr(MatchExpr &node)
{
    // Evaluate scrutinee (must be a Result type)
    node.scrutinee->Accept(*this);
    llvm::Value *scrutineeValue = m_LastValue;
    if (!scrutineeValue)
    {
        JLANG_ERROR("Invalid scrutinee in match expression");
        return;
    }

    // Get scrutinee type info - we need to determine the Result type
    // For now, scrutinee should be a variable reference to a Result type
    llvm::Type *scrutineeType = scrutineeValue->getType();
    if (!scrutineeType->isStructTy())
    {
        JLANG_ERROR("Match scrutinee must be a Result type");
        m_LastValue = nullptr;
        return;
    }

    llvm::StructType *resultStructType = llvm::cast<llvm::StructType>(scrutineeType);
    std::string typeName = resultStructType->getName().str();

    // Look up the Result type info
    ResultTypeInfo *resultInfo = m_symbols.LookupResultType(typeName);
    if (!resultInfo)
    {
        JLANG_ERROR(STR("Unknown Result type: %s", typeName.c_str()));
        m_LastValue = nullptr;
        return;
    }

    // Mark the variable as matched (for safety tracking)
    if (auto *varExpr = dynamic_cast<VarExpr *>(node.scrutinee.get()))
    {
        VariableInfo *varInfo = m_symbols.LookupVariable(varExpr->name);
        if (varInfo)
        {
            varInfo->isResultMatched = true;
        }
    }

    // We need to store the scrutinee to extract fields
    llvm::AllocaInst *scrutineeAlloca =
        m_IRBuilder.CreateAlloca(resultStructType, nullptr, "match_scrutinee");
    m_IRBuilder.CreateStore(scrutineeValue, scrutineeAlloca);

    // Extract tag
    llvm::Value *tagPtr = m_IRBuilder.CreateStructGEP(resultStructType, scrutineeAlloca, 0, "tag_ptr");
    llvm::Value *tag = m_IRBuilder.CreateLoad(llvm::Type::getInt8Ty(m_Context), tagPtr, "tag");

    // Compare tag to 0 (Ok)
    llvm::Value *isOk =
        m_IRBuilder.CreateICmpEQ(tag, llvm::ConstantInt::get(llvm::Type::getInt8Ty(m_Context), 0), "is_ok");

    // Create basic blocks
    llvm::Function *parentFunc = m_IRBuilder.GetInsertBlock()->getParent();
    llvm::BasicBlock *okBlock = llvm::BasicBlock::Create(m_Context, "match.ok", parentFunc);
    llvm::BasicBlock *errBlock = llvm::BasicBlock::Create(m_Context, "match.err");
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(m_Context, "match.merge");

    m_IRBuilder.CreateCondBr(isOk, okBlock, errBlock);

    // Ok arm
    m_IRBuilder.SetInsertPoint(okBlock);

    // Extract Ok value and bind to variable
    llvm::Value *dataPtr = m_IRBuilder.CreateStructGEP(resultStructType, scrutineeAlloca, 1, "data_ptr");
    llvm::Type *okType = MapType(resultInfo->okType);
    llvm::Value *okDataPtr =
        m_IRBuilder.CreateBitCast(dataPtr, llvm::PointerType::getUnqual(okType), "ok_data_ptr");
    llvm::Value *okValue = m_IRBuilder.CreateLoad(okType, okDataPtr, "ok_value");

    // Create binding variable for Ok arm
    llvm::AllocaInst *okBindingAlloca = m_IRBuilder.CreateAlloca(okType, nullptr, node.okArm.bindingName);
    m_IRBuilder.CreateStore(okValue, okBindingAlloca);
    m_symbols.DefineVariable(node.okArm.bindingName,
                             VariableInfo{okBindingAlloca, resultInfo->okType, false, false});
    m_symbols.TrackFunctionLocal(node.okArm.bindingName);

    // Execute Ok arm body
    node.okArm.body->Accept(*this);
    llvm::Value *okResult = m_LastValue;

    // Clean up Ok binding
    m_symbols.EraseVariable(node.okArm.bindingName);

    llvm::BasicBlock *okExitBlock = m_IRBuilder.GetInsertBlock();
    m_IRBuilder.CreateBr(mergeBlock);

    // Err arm
    errBlock->insertInto(parentFunc);
    m_IRBuilder.SetInsertPoint(errBlock);

    // Extract Err value and bind to variable
    llvm::Type *errType = MapType(resultInfo->errType);
    llvm::Value *errDataPtr =
        m_IRBuilder.CreateBitCast(dataPtr, llvm::PointerType::getUnqual(errType), "err_data_ptr");
    llvm::Value *errValue = m_IRBuilder.CreateLoad(errType, errDataPtr, "err_value");

    // Create binding variable for Err arm
    llvm::AllocaInst *errBindingAlloca = m_IRBuilder.CreateAlloca(errType, nullptr, node.errArm.bindingName);
    m_IRBuilder.CreateStore(errValue, errBindingAlloca);
    m_symbols.DefineVariable(node.errArm.bindingName,
                             VariableInfo{errBindingAlloca, resultInfo->errType, false, false});
    m_symbols.TrackFunctionLocal(node.errArm.bindingName);

    // Execute Err arm body
    node.errArm.body->Accept(*this);
    llvm::Value *errResult = m_LastValue;

    // Clean up Err binding
    m_symbols.EraseVariable(node.errArm.bindingName);

    llvm::BasicBlock *errExitBlock = m_IRBuilder.GetInsertBlock();
    m_IRBuilder.CreateBr(mergeBlock);

    // Merge block with PHI node if arms return values
    mergeBlock->insertInto(parentFunc);
    m_IRBuilder.SetInsertPoint(mergeBlock);

    if (okResult && errResult && okResult->getType() == errResult->getType() &&
        !okResult->getType()->isVoidTy())
    {
        llvm::PHINode *phi = m_IRBuilder.CreatePHI(okResult->getType(), 2, "match_result");
        phi->addIncoming(okResult, okExitBlock);
        phi->addIncoming(errResult, errExitBlock);
        m_LastValue = phi;
    }
    else
    {
        m_LastValue = nullptr;
    }
}

void CodeGenerator::VisitSwitchExpr(SwitchExpr &node)
{
    // 1. Evaluate switch expression
    node.expr->Accept(*this);
    llvm::Value *switchValue = m_LastValue;
    if (!switchValue)
    {
        JLANG_ERROR("Invalid switch expression");
        return;
    }

    llvm::Function *parentFunc = m_IRBuilder.GetInsertBlock()->getParent();

    // 2. Create exit block (merge point)
    llvm::BasicBlock *exitBlock = llvm::BasicBlock::Create(m_Context, "switchexpr.exit");

    // 3. Find default arm index
    int defaultIndex = -1;
    for (size_t i = 0; i < node.arms.size(); ++i)
    {
        if (node.arms[i].isDefault)
        {
            defaultIndex = static_cast<int>(i);
            break;
        }
    }

    // 4. Create basic blocks for each arm
    std::vector<llvm::BasicBlock *> armBlocks;
    for (size_t i = 0; i < node.arms.size(); ++i)
    {
        std::string name = node.arms[i].isDefault ? "switchexpr.default" : "switchexpr.arm";
        armBlocks.push_back(llvm::BasicBlock::Create(m_Context, name));
    }

    llvm::BasicBlock *defaultBlock = (defaultIndex >= 0) ? armBlocks[defaultIndex] : exitBlock;

    // 5. Count case values and create switch
    unsigned numCaseValues = 0;
    for (auto &arm : node.arms)
    {
        if (!arm.isDefault)
            numCaseValues += arm.values.size();
    }

    llvm::SwitchInst *switchInst = m_IRBuilder.CreateSwitch(switchValue, defaultBlock, numCaseValues);

    // 6. Add case values
    for (size_t i = 0; i < node.arms.size(); ++i)
    {
        if (node.arms[i].isDefault)
            continue;

        for (auto &val : node.arms[i].values)
        {
            val->Accept(*this);
            llvm::ConstantInt *constVal = llvm::dyn_cast<llvm::ConstantInt>(m_LastValue);
            if (!constVal)
            {
                JLANG_ERROR("Switch expression case value must be a compile-time constant");
                continue;
            }
            switchInst->addCase(constVal, armBlocks[i]);
        }
    }

    // 7. Generate code for each arm, collect results for PHI
    std::vector<std::pair<llvm::Value *, llvm::BasicBlock *>> phiIncoming;

    for (size_t i = 0; i < node.arms.size(); ++i)
    {
        armBlocks[i]->insertInto(parentFunc);
        m_IRBuilder.SetInsertPoint(armBlocks[i]);

        if (node.arms[i].body)
        {
            node.arms[i].body->Accept(*this);
        }

        llvm::Value *armResult = m_LastValue;
        llvm::BasicBlock *armExitBlock = m_IRBuilder.GetInsertBlock();

        if (!armExitBlock->getTerminator())
        {
            m_IRBuilder.CreateBr(exitBlock);
        }

        if (armResult)
        {
            phiIncoming.push_back({armResult, armExitBlock});
        }
    }

    // 8. Create PHI node in exit block
    exitBlock->insertInto(parentFunc);
    m_IRBuilder.SetInsertPoint(exitBlock);

    if (!phiIncoming.empty())
    {
        llvm::PHINode *phi =
            m_IRBuilder.CreatePHI(phiIncoming[0].first->getType(), phiIncoming.size(), "switch_result");
        for (auto &[val, block] : phiIncoming)
        {
            phi->addIncoming(val, block);
        }
        m_LastValue = phi;
    }
    else
    {
        m_LastValue = nullptr;
    }
}

} // namespace jlang
