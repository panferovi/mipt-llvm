#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <string>

#include "../GameLife/graphic_lib.c"

using namespace llvm;

void GenerateCellField_Initialize(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);
void GenerateCellField_Update(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);
void GenerateCellField_Copy(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);
void GenerateCellField_MarkCell(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);
void GenerateCellField_SetNearbyCells(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);
void GenerateCellField_IsCellDead(LLVMContext &context, Module *module, IRBuilder<> &builder);
void GenerateSetPixels(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);
void GenerateMain(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field);

int main()
{
    LLVMContext context;
    Module *module = new Module("app", context);
    IRBuilder<> builder(context);

    auto *cell_field = StructType::create(context,
                                          {ArrayType::get(ArrayType::get(builder.getInt32Ty(), 400), 400)}, "struct.CellField");

    GenerateCellField_Initialize(context, module, builder, cell_field);
    GenerateCellField_MarkCell(context, module, builder, cell_field);
    GenerateCellField_Update(context, module, builder, cell_field);
    GenerateCellField_Copy(context, module, builder, cell_field);
    GenerateCellField_SetNearbyCells(context, module, builder, cell_field);
    GenerateCellField_IsCellDead(context, module, builder);
    GenerateSetPixels(context, module, builder, cell_field);
    GenerateMain(context, module, builder, cell_field);

    module->print(outs(), nullptr);

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();

    ee->InstallLazyFunctionCreator([&](const std::string &function) -> void * {
        if (function == "CreateGraphicResources") {
            return reinterpret_cast<void *>(CreateGraphicResources);
        }
        if (function == "DestroyGraphicResources") {
            return reinterpret_cast<void *>(DestroyGraphicResources);
        }
        if (function == "RenderWindowIsOpen") {
            return reinterpret_cast<void *>(RenderWindowIsOpen);
        }
        if (function == "ClearFrame") {
            return reinterpret_cast<void *>(ClearFrame);
        }
        if (function == "DisplayFrame") {
            return reinterpret_cast<void *>(DisplayFrame);
        }
        if (function == "SetPixel") {
            return reinterpret_cast<void *>(SetPixel);
        }
        if (function == "GetRandomInteger") {
            return reinterpret_cast<void *>(GetRandomInteger);
        }
        return nullptr;
    });

    ee->finalizeObject();
    ArrayRef<GenericValue> noargs;
    GenericValue v = ee->runFunction(module->getFunction("Main"), noargs);

    return 0;
}

void GenerateCellField_Initialize(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto generate_static_i32 = [&](std::string name, unsigned value)
    {
        module->getOrInsertGlobal(name, builder.getInt32Ty());
        GlobalVariable *var = module->getNamedGlobal(name);
        var->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
        var->setAlignment(Align(4));
        var->setDSOLocal(true);
        var->setInitializer(ConstantInt::get(builder.getInt32Ty(), value, true));
    };

    generate_static_i32("CellField_dead_cell", 0);
    generate_static_i32("CellField_live_cell", 1);

    auto *cf_initialize = Function::Create(FunctionType::get(builder.getVoidTy(), {cell_field->getPointerTo()}, false),
                                           Function::ExternalLinkage, "CellField_Initialize", module);
    cf_initialize->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    cf_initialize->setDSOLocal(true);
    cf_initialize->addFnAttr(Attribute::AttrKind::NoUnwind);
    cf_initialize->addFnAttr(Attribute::AttrKind::UWTable);
    cf_initialize->addParamAttr(0, Attribute::AttrKind::NoCapture);
    cf_initialize->addParamAttr(0, Attribute::AttrKind::NoUndef);
    cf_initialize->addParamAttr(0, Attribute::AttrKind::WriteOnly);

    auto *get_rand_i32 = Function::Create(FunctionType::get(builder.getInt32Ty(), false),
                                          Function::ExternalLinkage, "GetRandomInteger", module);
    get_rand_i32->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);

    auto *bb1 = BasicBlock::Create(context, "", cf_initialize);
    auto *bb2 = BasicBlock::Create(context, "", cf_initialize);
    auto *bb4 = BasicBlock::Create(context, "", cf_initialize);
    auto *bb5 = BasicBlock::Create(context, "", cf_initialize);
    auto *bb8 = BasicBlock::Create(context, "", cf_initialize);

    builder.SetInsertPoint(bb1);
    builder.CreateBr(bb2);

    builder.SetInsertPoint(bb2);
    auto *val3 = builder.CreatePHI(builder.getInt64Ty(), 2);
    builder.CreateBr(bb8);

    builder.SetInsertPoint(bb4);
    builder.CreateRetVoid();

    builder.SetInsertPoint(bb5);
    auto *val6 = builder.CreateAdd(val3, builder.getInt64(1), "", true, true);
    auto *val7 = builder.CreateICmpEQ(val6, builder.getInt64(400));
    builder.CreateCondBr(val7, bb4, bb2);

    builder.SetInsertPoint(bb8);
    auto *val9 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val10 = builder.CreateCall(get_rand_i32);
    val10->setTailCall(true);
    auto *val11 = builder.CreateSRem(val10, builder.getInt32(5));
    auto *val12 = builder.CreateICmpEQ(val11, builder.getInt32(0));
    auto *val13 = builder.CreateZExt(val12, builder.getInt32Ty());
    auto *val14 = builder.CreateInBoundsGEP(cell_field, cf_initialize->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val3, val9});
    builder.CreateStore(val13, val14)->setAlignment(Align(4));
    auto *val15 = builder.CreateAdd(val9, builder.getInt64(1), "", true, true);
    auto *val16 = builder.CreateICmpEQ(val15, builder.getInt64(400));
    builder.CreateCondBr(val16, bb5, bb8);

    val3->addIncoming(builder.getInt64(0), bb1);
    val3->addIncoming(val6, bb5);

    val9->addIncoming(builder.getInt64(0), bb2);
    val9->addIncoming(val15, bb8);
}

void GenerateCellField_Update(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto *cell_field_p = cell_field->getPointerTo();
    auto *cf_update = Function::Create(FunctionType::get(builder.getVoidTy(), {cell_field_p}, false),
                                       Function::ExternalLinkage, "CellField_Update", module);
    cf_update->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    cf_update->setDSOLocal(true);
    cf_update->addFnAttr(Attribute::AttrKind::NoFree);
    cf_update->addFnAttr(Attribute::AttrKind::NoSync);
    cf_update->addFnAttr(Attribute::AttrKind::NoUnwind);
    cf_update->addFnAttr(Attribute::AttrKind::UWTable);
    cf_update->addParamAttr(0, Attribute::AttrKind::NoCapture);
    cf_update->addParamAttr(0, Attribute::AttrKind::NoUndef);

    auto *bb1 = BasicBlock::Create(context, "", cf_update);
    auto *bb5 = BasicBlock::Create(context, "", cf_update);
    auto *bb7 = BasicBlock::Create(context, "", cf_update);
    auto *bb10 = BasicBlock::Create(context, "", cf_update);
    auto *bb11 = BasicBlock::Create(context, "", cf_update);

    builder.SetInsertPoint(bb1);
    auto *val2 = builder.CreateBitCast(cf_update->getArg(0), builder.getInt8PtrTy());
    auto *val3 = builder.CreateAlloca(cell_field);
    val3->setAlignment(Align(4));
    auto *val4 = builder.CreateBitCast(val3, builder.getInt8PtrTy());

    auto *lifetime_start = builder.CreateIntrinsic(Intrinsic::lifetime_start, {builder.getInt8PtrTy()},
                                                   {builder.getInt64(640000), val4});
    lifetime_start->addParamAttr(1, Attribute::AttrKind::NonNull);

    auto create_memcpy_intrinsic = [&](Value *val1, Value *val2) {
        auto *memcpy = builder.CreateIntrinsic(Intrinsic::memcpy,
                                               {builder.getInt8PtrTy(), builder.getInt8PtrTy(), builder.getInt64Ty()},
                                               {val1, val2, builder.getInt64(640000), builder.getInt1(false)});
        memcpy->addParamAttr(0, Attribute::AttrKind::NoUndef);
        memcpy->addParamAttr(0, Attribute::AttrKind::NonNull);
        memcpy->addParamAttr(0, Attribute::getWithAlignment(context, Align(4)));
        memcpy->addParamAttr(0, Attribute::getWithDereferenceableBytes(context, 640000));
        memcpy->addParamAttr(1, Attribute::AttrKind::NoUndef);
        memcpy->addParamAttr(1, Attribute::AttrKind::NonNull);
        memcpy->addParamAttr(1, Attribute::getWithAlignment(context, Align(4)));
        memcpy->addParamAttr(1, Attribute::getWithDereferenceableBytes(context, 640000));

    };
    create_memcpy_intrinsic(val4, val2);
    builder.CreateBr(bb5);

    builder.SetInsertPoint(bb5);
    auto *val6 = builder.CreatePHI(builder.getInt32Ty(), 2);
    builder.CreateBr(bb11);

    builder.SetInsertPoint(bb7);
    auto *val8 = builder.CreateAdd(val6, builder.getInt32(1), "", true, true);
    auto *val9 = builder.CreateICmpEQ(val8, builder.getInt32(400));
    builder.CreateCondBr(val9, bb10, bb5);

    builder.SetInsertPoint(bb10);
    create_memcpy_intrinsic(val2, val4);
    auto *lifetime_end = builder.CreateIntrinsic(Intrinsic::lifetime_end,
                                                   {builder.getInt8PtrTy()},
                                                   {builder.getInt64(640000), val4});
    lifetime_end->addParamAttr(1, Attribute::AttrKind::NonNull);
    builder.CreateRetVoid();

    builder.SetInsertPoint(bb11);
    auto *val12 = builder.CreatePHI(builder.getInt32Ty(), 2);

    auto cf_mark_cell = module->getOrInsertFunction("CellField_MarkCell", FunctionType::get(builder.getVoidTy(),
                                                    {cell_field_p, cell_field_p, builder.getInt32Ty(), builder.getInt32Ty()}, false));

    auto *mark_cell_call = builder.CreateCall(cf_mark_cell, {cf_update->getArg(0), val3, val6, val12});
    mark_cell_call->addParamAttr(0, Attribute::AttrKind::NoUndef);
    mark_cell_call->addParamAttr(1, Attribute::AttrKind::NoUndef);
    mark_cell_call->addParamAttr(1, Attribute::AttrKind::NonNull);
    mark_cell_call->addParamAttr(2, Attribute::AttrKind::NoUndef);
    mark_cell_call->addParamAttr(3, Attribute::AttrKind::NoUndef);
    auto *val13 = builder.CreateAdd(val12, builder.getInt32(1), "", true, true);
    auto *val14 = builder.CreateICmpEQ(val13, builder.getInt32(400));
    builder.CreateCondBr(val14, bb7, bb11);

    val6->addIncoming(val8, bb7);
    val6->addIncoming(builder.getInt32(0), bb1);

    val12->addIncoming(builder.getInt32(0), bb5);
    val12->addIncoming(val13, bb11);
}

void GenerateCellField_Copy(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto *cell_field_p = cell_field->getPointerTo();
    auto *cf_copy = Function::Create(FunctionType::get(builder.getVoidTy(), {cell_field_p, cell_field_p}, false),
                                     Function::ExternalLinkage, "CellField_Copy", module);
    cf_copy->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    cf_copy->setDSOLocal(true);
    cf_copy->addFnAttr(Attribute::AttrKind::NoFree);
    cf_copy->addFnAttr(Attribute::AttrKind::NoRecurse);
    cf_copy->addFnAttr(Attribute::AttrKind::NoSync);
    cf_copy->addFnAttr(Attribute::AttrKind::NoUnwind);
    cf_copy->addFnAttr(Attribute::AttrKind::UWTable);
    cf_copy->addParamAttr(0, Attribute::AttrKind::NoCapture);
    cf_copy->addParamAttr(0, Attribute::AttrKind::NoUndef);
    cf_copy->addParamAttr(0, Attribute::AttrKind::ReadOnly);
    cf_copy->addParamAttr(1, Attribute::AttrKind::NoCapture);
    cf_copy->addParamAttr(1, Attribute::AttrKind::NoUndef);
    cf_copy->addParamAttr(1, Attribute::AttrKind::WriteOnly);

    auto *bb2 = BasicBlock::Create(context, "", cf_copy);
    auto *bb3 = BasicBlock::Create(context, "", cf_copy);
    auto *bb13 = BasicBlock::Create(context, "", cf_copy);
    auto *bb38 = BasicBlock::Create(context, "", cf_copy);
    auto *bb39 = BasicBlock::Create(context, "", cf_copy);
    auto *bb42 = BasicBlock::Create(context, "", cf_copy);

    builder.SetInsertPoint(bb2);
    builder.CreateBr(bb3);

    builder.SetInsertPoint(bb3);
    auto *val4 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val5 = builder.CreateAdd(val4, builder.getInt64(1), "", true);
    auto *val6 = builder.CreateGEP(cell_field, cf_copy->getArg(0),
                                   {builder.getInt64(0), builder.getInt32(0), val5, builder.getInt64(0)});
    auto *val7 = builder.CreateGEP(cell_field, cf_copy->getArg(0),
                                   {builder.getInt64(0), builder.getInt32(0), val4, builder.getInt64(0)});
    auto *val8 = builder.CreateGEP(cell_field, cf_copy->getArg(1),
                                   {builder.getInt64(0), builder.getInt32(0), val5, builder.getInt64(0)});
    auto *val9 = builder.CreateGEP(cell_field, cf_copy->getArg(1),
                                   {builder.getInt64(0), builder.getInt32(0), val4, builder.getInt64(0)});
    auto *val10 = builder.CreateICmpULT(val9, val6);
    auto *val11 = builder.CreateICmpULT(val7, val8);
    auto *val12 = builder.CreateAnd(val10, val11);

    builder.SetInsertPoint(bb13);
    auto *vec_type = VectorType::get(builder.getInt32Ty(), ElementCount::get(4, false));
    auto *val14 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val15 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val14});
    auto *val16 = builder.CreateBitCast(val15, vec_type->getPointerTo());
    auto *val17 = builder.CreateLoad(vec_type, val16);
    val17->setAlignment(Align(4));
    auto *val18 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val15, builder.getInt64(4));
    auto *val19 = builder.CreateBitCast(val18, vec_type->getPointerTo());
    auto *val20 = builder.CreateLoad(vec_type, val19);
    val20->setAlignment(Align(4));
    auto *val21 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(1),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val14});
    auto *val22 = builder.CreateBitCast(val21, vec_type->getPointerTo());
    builder.CreateStore(val17, val22)->setAlignment(Align(4));
    auto *val23 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val21, builder.getInt64(4));
    auto *val24 = builder.CreateBitCast(val23, vec_type->getPointerTo());
    builder.CreateStore(val20, val24)->setAlignment(Align(4));
    auto *val25 = builder.CreateOr(val14, builder.getInt64(8));
    auto *val26 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val25});
    auto *val27 = builder.CreateBitCast(val26, vec_type->getPointerTo());
    auto *val28 = builder.CreateLoad(vec_type, val27);
    val28->setAlignment(Align(4));
    auto *val29 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val26, builder.getInt64(4));
    auto *val30 = builder.CreateBitCast(val29, vec_type->getPointerTo());
    auto *val31 = builder.CreateLoad(vec_type, val30);
    val31->setAlignment(Align(4));
    auto *val32 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(1),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val25});
    auto *val33 = builder.CreateBitCast(val32, vec_type->getPointerTo());
    builder.CreateStore(val28, val33)->setAlignment(Align(4));
    auto *val34 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val32, builder.getInt64(4));
    auto *val35 = builder.CreateBitCast(val34, vec_type->getPointerTo());
    builder.CreateStore(val31, val35)->setAlignment(Align(4));
    auto *val36 = builder.CreateAdd(val14, builder.getInt64(16), "", true, true);
    auto *val37 = builder.CreateICmpEQ(val36, builder.getInt64(400));

    builder.SetInsertPoint(bb38);
    builder.CreateRetVoid();

    builder.SetInsertPoint(bb39);
    auto *val40 = builder.CreateAdd(val4, builder.getInt64(1), "", true, true);
    auto *val41 = builder.CreateICmpEQ(val40, builder.getInt64(400));
    builder.CreateCondBr(val41, bb38, bb3);

    builder.SetInsertPoint(bb42);
    auto *val43 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val44 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val43});
    auto *val45 = builder.CreateLoad(builder.getInt32Ty(), val44);
    val45->setAlignment(Align(4));
    auto *val46 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(1),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val43});
    builder.CreateStore(val45, val46)->setAlignment(Align(4));
    auto *val47 = builder.CreateOr(val43, builder.getInt64(1));
    auto *val48 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val47});
    auto *val49 = builder.CreateLoad(builder.getInt32Ty(), val48);
    val49->setAlignment(Align(4));
    auto *val50 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(1),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val47});
    builder.CreateStore(val49, val50)->setAlignment(Align(4));
    auto *val51 = builder.CreateOr(val43, builder.getInt64(2));
    auto *val52 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val51});
    auto *val53 = builder.CreateLoad(builder.getInt32Ty(), val52);
    val53->setAlignment(Align(4));
    auto *val54 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(1),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val51});
    builder.CreateStore(val53, val54)->setAlignment(Align(4));
    auto *val55 = builder.CreateOr(val43, builder.getInt64(3));
    auto *val56 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(0),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val55});
    auto *val57 = builder.CreateLoad(builder.getInt32Ty(), val56);
    val57->setAlignment(Align(4));
    auto *val58 = builder.CreateInBoundsGEP(cell_field, cf_copy->getArg(1),
                                            {builder.getInt64(0), builder.getInt32(0), val4, val55});
    builder.CreateStore(val57, val58)->setAlignment(Align(4));
    auto *val59 = builder.CreateAdd(val43, builder.getInt64(4), "", true, true);
    auto *val60 = builder.CreateICmpEQ(val59, builder.getInt64(400));
    builder.CreateCondBr(val60, bb39, bb42);

    builder.SetInsertPoint(bb3);
    builder.CreateCondBr(val12, bb42, bb13);

    builder.SetInsertPoint(bb13);
    builder.CreateCondBr(val37, bb39, bb13);

    val4->addIncoming(builder.getInt64(0), bb2);
    val4->addIncoming(val40, bb39);

    val14->addIncoming(val36, bb13);
    val14->addIncoming(builder.getInt64(0), bb3);

    val43->addIncoming(val59, bb42);
    val43->addIncoming(builder.getInt64(0), bb3);
}

void GenerateCellField_MarkCell(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto *cell_field_p = cell_field->getPointerTo();
    auto *cf_mark_cell = Function::Create(FunctionType::get(builder.getVoidTy(),
                                          {cell_field_p, cell_field_p, builder.getInt32Ty(), builder.getInt32Ty()}, false),
                                          Function::ExternalLinkage, "CellField_MarkCell", module);
    cf_mark_cell->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    cf_mark_cell->setDSOLocal(true);
    cf_mark_cell->addFnAttr(Attribute::AttrKind::NoFree);
    cf_mark_cell->addFnAttr(Attribute::AttrKind::NoSync);
    cf_mark_cell->addFnAttr(Attribute::AttrKind::NoUnwind);
    cf_mark_cell->addFnAttr(Attribute::AttrKind::UWTable);
    cf_mark_cell->addParamAttr(0, Attribute::AttrKind::NoCapture);
    cf_mark_cell->addParamAttr(0, Attribute::AttrKind::NoUndef);
    cf_mark_cell->addParamAttr(0, Attribute::AttrKind::ReadOnly);
    cf_mark_cell->addParamAttr(1, Attribute::AttrKind::NoCapture);
    cf_mark_cell->addParamAttr(1, Attribute::AttrKind::NoUndef);
    cf_mark_cell->addParamAttr(1, Attribute::AttrKind::WriteOnly);
    cf_mark_cell->addParamAttr(2, Attribute::AttrKind::NoUndef);
    cf_mark_cell->addParamAttr(3, Attribute::AttrKind::NoUndef);

    auto *bb4 = BasicBlock::Create(context, "", cf_mark_cell);
    auto *bb66 = BasicBlock::Create(context, "", cf_mark_cell);
    auto *bb68 = BasicBlock::Create(context, "", cf_mark_cell);
    auto *bb70 = BasicBlock::Create(context, "", cf_mark_cell);
    auto *bb73 = BasicBlock::Create(context, "", cf_mark_cell);
    auto *bb76 = BasicBlock::Create(context, "", cf_mark_cell);

    auto *val0 = cf_mark_cell->getArg(0);
    auto *val1 = cf_mark_cell->getArg(1);
    auto *val2 = cf_mark_cell->getArg(2);
    auto *val3 = cf_mark_cell->getArg(3);

    builder.SetInsertPoint(bb4);
    auto *val5 = builder.CreateICmpEQ(val2, builder.getInt32(0));
    auto *val6 = builder.CreateAdd(val2, builder.getInt32(-1), "", false, true);
    auto *val7 = builder.CreateSelect(val5, builder.getInt32(399), val6);
    auto *val8 = builder.CreateICmpEQ(val2, builder.getInt32(399));
    auto *val9 = builder.CreateAdd(val2, builder.getInt32(1), "", false, true);
    auto *val10 = builder.CreateSelect(val8, builder.getInt32(0), val9);
    auto *val11 = builder.CreateICmpEQ(val3, builder.getInt32(0));
    auto *val12 = builder.CreateAdd(val3, builder.getInt32(-1), "", false, true);
    auto *val13 = builder.CreateSelect(val11, builder.getInt32(399), val12);
    auto *val14 = builder.CreateICmpEQ(val3, builder.getInt32(399));
    auto *val15 = builder.CreateAdd(val3, builder.getInt32(1), "", false, true);
    auto *val16 = builder.CreateSelect(val14, builder.getInt32(0), val15);
    auto *val17 = builder.CreateSExt(val7, builder.getInt64Ty());
    auto *val18 = builder.CreateSExt(val13, builder.getInt64Ty());
    auto *val19 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val17, val18});
    auto *val20 = builder.CreateLoad(builder.getInt32Ty(), val19);
    val20->setAlignment(Align(4));
    auto *val21 = builder.CreateSExt(val3, builder.getInt64Ty());
    auto *val22 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val17, val21});
    auto *val23 = builder.CreateLoad(builder.getInt32Ty(), val22);
    val23->setAlignment(Align(4));
    auto *val24 = builder.CreateSExt(val16, builder.getInt64Ty());
    auto *val25 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val17, val24});
    auto *val26 = builder.CreateLoad(builder.getInt32Ty(), val25);
    val26->setAlignment(Align(4));
    auto *val27 = builder.CreateSExt(val2, builder.getInt64Ty());

    auto *val28 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val27, val18});
    auto *val29 = builder.CreateLoad(builder.getInt32Ty(), val28);
    val29->setAlignment(Align(4));
    auto *val30 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val27, val24});
    auto *val31 = builder.CreateLoad(builder.getInt32Ty(), val30);
    val31->setAlignment(Align(4));
    auto *val32 = builder.CreateSExt(val10, builder.getInt64Ty());
    auto *val33 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val32, val18});
    auto *val34 = builder.CreateLoad(builder.getInt32Ty(), val33);
    val34->setAlignment(Align(4));
    auto *val35 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val32, val21});
    auto *val36 = builder.CreateLoad(builder.getInt32Ty(), val35);
    val36->setAlignment(Align(4));
    auto *val37 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val32, val24});
    auto *val38 = builder.CreateLoad(builder.getInt32Ty(), val37);
    val38->setAlignment(Align(4));
    auto *val39 = builder.CreateLoad(builder.getInt32Ty(), module->getNamedValue("CellField_dead_cell"));
    val39->setAlignment(Align(4));
    auto *val40 = builder.CreateICmpNE(val39, val20);
    auto *val41 = builder.CreateZExt(val40, builder.getInt32Ty());
    auto *val42 = builder.CreateICmpNE(val39, val23);
    auto *val43 = builder.CreateZExt(val42, builder.getInt32Ty());
    auto *val44 = builder.CreateAdd(val41, val43, "", true, true);
    auto *val45 = builder.CreateICmpNE(val39, val26);
    auto *val46 = builder.CreateZExt(val45, builder.getInt32Ty());
    auto *val47 = builder.CreateAdd(val44, val46, "", true, true);
    auto *val48 = builder.CreateICmpNE(val39, val29);
    auto *val49 = builder.CreateZExt(val48, builder.getInt32Ty());
    auto *val50 = builder.CreateAdd(val47, val49, "", true, true);
    auto *val51 = builder.CreateICmpNE(val39, val31);
    auto *val52 = builder.CreateZExt(val51, builder.getInt32Ty());
    auto *val53 = builder.CreateAdd(val50, val52, "", true, true);
    auto *val54 = builder.CreateICmpNE(val39, val34);
    auto *val55 = builder.CreateZExt(val54, builder.getInt32Ty());
    auto *val56 = builder.CreateAdd(val53, val55, "", true, true);
    auto *val57 = builder.CreateICmpNE(val39, val36);
    auto *val58 = builder.CreateZExt(val57, builder.getInt32Ty());
    auto *val59 = builder.CreateAdd(val56, val58, "", true, true);
    auto *val60 = builder.CreateICmpNE(val39, val38);
    auto *val61 = builder.CreateZExt(val60, builder.getInt32Ty());
    auto *val62 = builder.CreateAdd(val59, val61, "", true, true);
    auto *val63 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val27, val21});
    auto *val64 = builder.CreateLoad(builder.getInt32Ty(), val63);
    val64->setAlignment(Align(4));
    auto *val65 = builder.CreateICmpEQ(val39, val64);

    builder.SetInsertPoint(bb66);
    auto *val67 = builder.CreateICmpEQ(val62, builder.getInt32(3));

    builder.SetInsertPoint(bb68);
    auto *val69 = builder.CreateLoad(builder.getInt32Ty(), module->getNamedValue("CellField_live_cell"));
    val69->setAlignment(Align(4));

    builder.SetInsertPoint(bb70);
    auto *val71 = builder.CreateAnd(val62, builder.getInt32(2147483646));
    auto *val72 = builder.CreateICmpEQ(val71, builder.getInt32(2));

    builder.SetInsertPoint(bb73);
    auto *val74 = builder.CreatePHI(builder.getInt32Ty(), 2);
    auto *val75 = builder.CreateInBoundsGEP(cell_field, val1,
                                            {builder.getInt64(0), builder.getInt32(0), val27, val21});
    builder.CreateStore(val74, val75)->setAlignment(Align(4));
    builder.CreateBr(bb76);

    builder.SetInsertPoint(bb76);
    builder.CreateRetVoid();

    builder.SetInsertPoint(bb4);
    builder.CreateCondBr(val65, bb66, bb70);

    builder.SetInsertPoint(bb66);
    builder.CreateCondBr(val67, bb68, bb76);

    builder.SetInsertPoint(bb68);
    builder.CreateBr(bb73);

    builder.SetInsertPoint(bb70);
    builder.CreateCondBr(val72, bb76, bb73);

    val74->addIncoming(val69, bb68);
    val74->addIncoming(val39, bb70);
}

void GenerateCellField_SetNearbyCells(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto *cell_field_p = cell_field->getPointerTo();
    auto *cf_set_nearby_cells = Function::Create(FunctionType::get(builder.getVoidTy(),
                                                 {cell_field_p, builder.getInt32Ty()->getPointerTo(), builder.getInt32Ty(), builder.getInt32Ty()}, false),
                                                 Function::ExternalLinkage, "CellField_SetNearbyCells", module);
    cf_set_nearby_cells->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    cf_set_nearby_cells->setDSOLocal(true);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::MustProgress);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::NoFree);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::NoRecurse);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::NoSync);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::NoUnwind);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::UWTable);
    cf_set_nearby_cells->addFnAttr(Attribute::AttrKind::WillReturn);
    cf_set_nearby_cells->addParamAttr(0, Attribute::AttrKind::NoCapture);
    cf_set_nearby_cells->addParamAttr(0, Attribute::AttrKind::NoUndef);
    cf_set_nearby_cells->addParamAttr(0, Attribute::AttrKind::ReadOnly);
    cf_set_nearby_cells->addParamAttr(1, Attribute::AttrKind::NoCapture);
    cf_set_nearby_cells->addParamAttr(1, Attribute::AttrKind::NoUndef);
    cf_set_nearby_cells->addParamAttr(1, Attribute::AttrKind::WriteOnly);
    cf_set_nearby_cells->addParamAttr(2, Attribute::AttrKind::NoUndef);
    cf_set_nearby_cells->addParamAttr(3, Attribute::AttrKind::NoUndef);

    auto *bb4 = BasicBlock::Create(context, "", cf_set_nearby_cells);

    auto *val0 = cf_set_nearby_cells->getArg(0);
    auto *val1 = cf_set_nearby_cells->getArg(1);
    auto *val2 = cf_set_nearby_cells->getArg(2);
    auto *val3 = cf_set_nearby_cells->getArg(3);

    builder.SetInsertPoint(bb4);

    auto *val5 = builder.CreateICmpEQ(val2, builder.getInt32(0));
    auto *val6 = builder.CreateAdd(val2, builder.getInt32(-1), "", false, true);
    auto *val7 = builder.CreateSelect(val5, builder.getInt32(399), val6);


    auto *val8 = builder.CreateICmpEQ(val2, builder.getInt32(399));
    auto *val9 = builder.CreateAdd(val2, builder.getInt32(1), "", false, true);
    auto *val10 = builder.CreateSelect(val8, builder.getInt32(0), val9);


    auto *val11 = builder.CreateICmpEQ(val3, builder.getInt32(0));
    auto *val12 = builder.CreateAdd(val3, builder.getInt32(-1), "", false, true);
    auto *val13 = builder.CreateSelect(val11, builder.getInt32(399), val12);

    auto *val14 = builder.CreateICmpEQ(val3, builder.getInt32(399));
    auto *val15 = builder.CreateAdd(val3, builder.getInt32(1), "", false, true);
    auto *val16 = builder.CreateSelect(val14, builder.getInt32(0), val15);

    auto *val17 = builder.CreateSExt(val7, builder.getInt64Ty());

    auto *val18 = builder.CreateSExt(val13, builder.getInt64Ty());
    auto *val19 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val17, val18});
    auto *val20 = builder.CreateLoad(builder.getInt32Ty(), val19);
    val20->setAlignment(Align(4));
    builder.CreateStore(val20, val1)->setAlignment(Align(4));

    auto *val21 = builder.CreateSExt(val3, builder.getInt64Ty());
    auto *val22 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val17, val21});
    auto *val23 = builder.CreateLoad(builder.getInt32Ty(), val22);
    val23->setAlignment(Align(4));

    auto *val24 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(1));
    builder.CreateStore(val23, val24)->setAlignment(Align(4));


    auto *val25 = builder.CreateSExt(val16, builder.getInt64Ty());
    auto *val26 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val17, val25});
    auto *val27 = builder.CreateLoad(builder.getInt32Ty(), val26);
    val27->setAlignment(Align(4));

    auto *val28 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(2));
    builder.CreateStore(val27, val28)->setAlignment(Align(4));

    auto *val29 = builder.CreateSExt(val2, builder.getInt64Ty());
    auto *val30 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val29, val18});
    auto *val31 = builder.CreateLoad(builder.getInt32Ty(), val30);
    val31->setAlignment(Align(4));

    auto *val32 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(3));
    builder.CreateStore(val31, val32)->setAlignment(Align(4));

    auto *val33 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val29, val25});
    auto *val34 = builder.CreateLoad(builder.getInt32Ty(), val33);
    val34->setAlignment(Align(4));
    auto *val35 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(4));
    builder.CreateStore(val34, val35)->setAlignment(Align(4));
    auto *val36 = builder.CreateSExt(val10, builder.getInt64Ty());
    auto *val37 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val36, val18});
    auto *val38 = builder.CreateLoad(builder.getInt32Ty(), val37);
    val38->setAlignment(Align(4));
    auto *val39 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(5));
    builder.CreateStore(val38, val39)->setAlignment(Align(4));
    auto *val40 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val36, val21});
    auto *val41 = builder.CreateLoad(builder.getInt32Ty(), val40);
    val41->setAlignment(Align(4));
    auto *val42 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(6));
    builder.CreateStore(val41, val42)->setAlignment(Align(4));
    auto *val43 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val36, val25});
    auto *val44 = builder.CreateLoad(builder.getInt32Ty(), val43);
    val44->setAlignment(Align(4));
    auto *val45 = builder.CreateInBoundsGEP(builder.getInt32Ty(), val1, builder.getInt64(7));
    builder.CreateStore(val44, val45)->setAlignment(Align(4));
    builder.CreateRetVoid();
}

void GenerateCellField_IsCellDead(LLVMContext &context, Module *module, IRBuilder<> &builder)
{
    auto *cf_is_cell_dead = Function::Create(FunctionType::get(builder.getInt32Ty(), {builder.getInt32Ty()}, false),
                                             Function::ExternalLinkage, "CellField_IsCellDead", module);
    cf_is_cell_dead->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    cf_is_cell_dead->setDSOLocal(true);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::MustProgress);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::NoFree);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::NoRecurse);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::NoSync);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::NoUnwind);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::ReadOnly);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::UWTable);
    cf_is_cell_dead->addFnAttr(Attribute::AttrKind::WillReturn);

    auto *bb1 = BasicBlock::Create(context, "", cf_is_cell_dead);

    auto *val0 = cf_is_cell_dead->getArg(0);

    builder.SetInsertPoint(bb1);
    auto *val2 = builder.CreateLoad(builder.getInt32Ty(), module->getNamedValue("CellField_dead_cell"));
    val2->setAlignment(Align(4));
    auto *val3 = builder.CreateICmpEQ(val2, val0);
    auto *val4 = builder.CreateZExt(val3, builder.getInt32Ty());
    builder.CreateRet(val4);
}

void GenerateSetPixels(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto *set_pixels = Function::Create(FunctionType::get(builder.getVoidTy(), {cell_field->getPointerTo()}, false),
                                        Function::ExternalLinkage, "SetPixels", module);
    set_pixels->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    set_pixels->setDSOLocal(true);
    set_pixels->addFnAttr(Attribute::AttrKind::NoUnwind);
    set_pixels->addFnAttr(Attribute::AttrKind::UWTable);
    set_pixels->addParamAttr(0, Attribute::AttrKind::NoCapture);
    set_pixels->addParamAttr(0, Attribute::AttrKind::NoUndef);
    set_pixels->addParamAttr(0, Attribute::AttrKind::ReadOnly);

    auto *bb1 = BasicBlock::Create(context, "", set_pixels);
    auto *bb2 = BasicBlock::Create(context, "", set_pixels);
    auto *bb5 = BasicBlock::Create(context, "", set_pixels);
    auto *bb6 = BasicBlock::Create(context, "", set_pixels);
    auto *bb9 = BasicBlock::Create(context, "", set_pixels);

    auto *val0 = set_pixels->getArg(0);

    builder.SetInsertPoint(bb1);
    builder.CreateBr(bb2);

    builder.SetInsertPoint(bb2);
    auto *val3 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val4 = builder.CreateTrunc(val3, builder.getInt32Ty());
    builder.CreateBr(bb9);

    builder.SetInsertPoint(bb5);
    builder.CreateRetVoid();

    builder.SetInsertPoint(bb6);
    auto *val7 = builder.CreateAdd(val3, builder.getInt64(1), "", true, true);
    auto *val8 = builder.CreateICmpEQ(val7, builder.getInt64(400));
    builder.CreateCondBr(val8, bb5, bb2);

    builder.SetInsertPoint(bb9);
    auto *val10 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val11 = builder.CreateInBoundsGEP(cell_field, val0,
                                            {builder.getInt64(0), builder.getInt32(0), val3, val10});
    auto *val12 = builder.CreateLoad(builder.getInt32Ty(), val11);
    val12->setAlignment(Align(4));
    auto *val13 = builder.CreateTrunc(val10, builder.getInt32Ty());

    auto set_pixel = module->getOrInsertFunction("SetPixel", builder.getVoidTy(),
                                                 builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty());
    auto set_pixel_call = builder.CreateCall(set_pixel, {val13, val4, val12});
    set_pixel_call->setTailCall(true);
    set_pixel_call->addParamAttr(0, Attribute::AttrKind::NoUndef);
    set_pixel_call->addParamAttr(1, Attribute::AttrKind::NoUndef);
    set_pixel_call->addParamAttr(2, Attribute::AttrKind::NoUndef);

    auto *val14 = builder.CreateAdd(val10, builder.getInt64(1), "", true, true);
    auto *val15 = builder.CreateICmpEQ(val14, builder.getInt64(400));
    builder.CreateCondBr(val15, bb6, bb9);

    val3->addIncoming(builder.getInt64(0), bb1);
    val3->addIncoming(val7, bb6);

    val10->addIncoming(builder.getInt64(0), bb2);
    val10->addIncoming(val14, bb9);
}

void GenerateMain(LLVMContext &context, Module *module, IRBuilder<> &builder, StructType *cell_field)
{
    auto *main = Function::Create(FunctionType::get(builder.getInt32Ty(), {}, false),
                                  Function::ExternalLinkage, "Main", module);
    main->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    main->setDSOLocal(true);
    main->addFnAttr(Attribute::AttrKind::NoUnwind);
    main->addFnAttr(Attribute::AttrKind::UWTable);

    auto *bb0 = BasicBlock::Create(context, "", main);
    auto *bb4 = BasicBlock::Create(context, "", main);
    auto *bb8 = BasicBlock::Create(context, "", main);
    auto *bb9 = BasicBlock::Create(context, "", main);
    auto *bb12 = BasicBlock::Create(context, "", main);
    auto *bb15 = BasicBlock::Create(context, "", main);
    auto *bb22 = BasicBlock::Create(context, "", main);
    auto *bb25 = BasicBlock::Create(context, "", main);
    auto *bb26 = BasicBlock::Create(context, "", main);

    builder.SetInsertPoint(bb0);
    auto *val1 = builder.CreateAlloca(cell_field);
    val1->setAlignment(Align(4));

    auto create_graphic_resources = module->getOrInsertFunction("CreateGraphicResources", builder.getInt32Ty(),
                                                                builder.getInt32Ty(), builder.getInt32Ty());
    auto *val2 = builder.CreateCall(create_graphic_resources, {builder.getInt32(400), builder.getInt32(400)});
    val2->setTailCall(true);
    val2->addParamAttr(0, Attribute::AttrKind::NoUndef);
    val2->addParamAttr(1, Attribute::AttrKind::NoUndef);
    auto *val3 = builder.CreateICmpEQ(val2, builder.getInt32(0));

    builder.SetInsertPoint(bb4);
    auto *val5 = builder.CreateBitCast(val1, builder.getInt8PtrTy());
    auto *lifetime_start = builder.CreateIntrinsic(Intrinsic::lifetime_start, {builder.getInt8PtrTy()},
                                                {builder.getInt64(640000), val5});
    auto cf_initialize_call = builder.CreateCall(module->getFunction("CellField_Initialize"), val1);
    cf_initialize_call->addParamAttr(0, Attribute::AttrKind::NoUndef);
    cf_initialize_call->addParamAttr(0, Attribute::AttrKind::NonNull);
    auto render_window_is_open = module->getOrInsertFunction("RenderWindowIsOpen", builder.getInt32Ty());
    auto *val6 = builder.CreateCall(render_window_is_open);
    auto *val7 = builder.CreateICmpEQ(val6, builder.getInt32(0));

    builder.SetInsertPoint(bb8);
    auto clear_frame = module->getOrInsertFunction("ClearFrame", builder.getVoidTy());
    builder.CreateCall(clear_frame);
    auto *cf_update_call = builder.CreateCall(module->getFunction("CellField_Update"), val1);
    cf_update_call->addParamAttr(0, Attribute::AttrKind::NoUndef);
    cf_update_call->addParamAttr(0, Attribute::AttrKind::NonNull);
    builder.CreateBr(bb9);

    builder.SetInsertPoint(bb9);
    auto *val10 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val11 = builder.CreateTrunc(val10, builder.getInt32Ty());

    builder.SetInsertPoint(bb12);
    auto *val13 = builder.CreateAdd(val10, builder.getInt64(1), "", true, true);
    auto *val14 = builder.CreateICmpEQ(val13, builder.getInt64(400));

    builder.SetInsertPoint(bb15);
    auto *val16 = builder.CreatePHI(builder.getInt64Ty(), 2);
    auto *val17 = builder.CreateInBoundsGEP(cell_field, val1,
                                            {builder.getInt64(0), builder.getInt32(0), val10, val16});
    auto *val18 = builder.CreateLoad(builder.getInt32Ty(), val17);
    val18->setAlignment(Align(4));
    auto *val19 = builder.CreateTrunc(val16, builder.getInt32Ty());
    auto set_pixel = module->getOrInsertFunction("SetPixel", builder.getVoidTy(),
                                                 builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty());
    auto set_pixel_call = builder.CreateCall(set_pixel, {val19, val11, val18});
    set_pixel_call->addParamAttr(0, Attribute::AttrKind::NoUndef);
    set_pixel_call->addParamAttr(1, Attribute::AttrKind::NoUndef);
    set_pixel_call->addParamAttr(2, Attribute::AttrKind::NoUndef);
    auto *val20 = builder.CreateAdd(val16, builder.getInt64(1), "", true, true);
    auto *val21 = builder.CreateICmpEQ(val20, builder.getInt64(400));
    builder.CreateCondBr(val21, bb12, bb15);

    builder.SetInsertPoint(bb22);
    auto display_frame = module->getOrInsertFunction("DisplayFrame", builder.getVoidTy());
    builder.CreateCall(display_frame);
    auto *val23 = builder.CreateCall(render_window_is_open);
    auto *val24 = builder.CreateICmpEQ(val23, builder.getInt32(0));
    builder.CreateCondBr(val24, bb25, bb8);

    builder.SetInsertPoint(bb25);
    auto destroy_graphic_resources = module->getOrInsertFunction("DestroyGraphicResources", builder.getVoidTy());
    builder.CreateCall(destroy_graphic_resources);
    auto *lifetime_end = builder.CreateIntrinsic(Intrinsic::lifetime_end, {builder.getInt8PtrTy()},
                                                   {builder.getInt64(640000), val5});
    builder.CreateBr(bb26);

    builder.SetInsertPoint(bb26);
    auto *val27 = builder.CreatePHI(builder.getInt32Ty(), 2);
    builder.CreateRet(val27);

    builder.SetInsertPoint(bb0);
    builder.CreateCondBr(val3, bb26, bb4);

    builder.SetInsertPoint(bb4);
    builder.CreateCondBr(val7, bb25, bb8);

    builder.SetInsertPoint(bb9);
    builder.CreateBr(bb15);

    builder.SetInsertPoint(bb12);
    builder.CreateCondBr(val14, bb22, bb9);

    val10->addIncoming(builder.getInt64(0), bb8);
    val10->addIncoming(val13, bb12);

    val16->addIncoming(builder.getInt64(0), bb9);
    val16->addIncoming(val20, bb15);

    val27->addIncoming(builder.getInt32(0), bb25);
    val27->addIncoming(builder.getInt32(1), bb0);
}
