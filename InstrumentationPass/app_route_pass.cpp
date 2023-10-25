#include <sstream>
#include <string_view>

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

namespace {

class AppRoutePass : public llvm::FunctionPass {
public:
    static char ID;
    AppRoutePass() : llvm::FunctionPass(ID) {}

    virtual bool runOnFunction(llvm::Function &function) {
        using namespace llvm;

        LLVMContext &ctx = function.getContext();
        IRBuilder<> builder(ctx);

        Type *ret_type = Type::getVoidTy(ctx);
        ArrayRef<Type *> route_params = {Type::getInt8PtrTy(ctx), Type::getInt64Ty(ctx)};
        FunctionType *route_signature = FunctionType::get(ret_type, route_params, false);
        FunctionCallee route_method = function.getParent()->getOrInsertFunction("RouteInstruction", route_signature);

        FunctionType *dump_signature = FunctionType::get(ret_type, {}, false);
        FunctionCallee dump_method = function.getParent()->getOrInsertFunction("DumpInstructions", dump_signature);

        for (auto &&basic_block : function) {
            for (auto &&instruction : basic_block) {
                if (dyn_cast<PHINode>(&instruction)) {
                    continue;
                }
                builder.SetInsertPoint(&instruction);
                Value *instr_dump = builder.CreateGlobalStringPtr(instruction.getOpcodeName());
                Value *instr_opcode = ConstantInt::get(builder.getInt64Ty(), instruction.getOpcode());
                builder.CreateCall(route_method, {instr_dump, instr_opcode});
                if (auto *main_ret = dyn_cast<ReturnInst>(&instruction); main_ret && function.getName() == "main") {
                    builder.CreateCall(dump_method, {});
                }
            }
        }
        return false;
    }
};

}  // namespace

char AppRoutePass::ID = 0;

static void registerMyPass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM)
{
    PM.add(new AppRoutePass());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_OptimizerLast, registerMyPass);
