#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
    class MemoryAllocationDetector : public PassInfoMixin<MemoryAllocationDetector>
    {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &)
        {
            for (BasicBlock &BB : F)
            {
                for (Instruction &I : BB)
                {
                    if (CallInst *CI = dyn_cast<CallInst>(&I))
                    {
                        if (Function *Callee = CI->getCalledFunction())
                        {
                            StringRef FuncName = Callee->getName();
                            if (FuncName == "malloc" || FuncName == "calloc" ||
                                FuncName == "realloc" || FuncName == "free")
                            {
                                errs() << "Memory allocation/deallocation detected: "
                                       << FuncName << " in function " << F.getName() << "\n";
                            }
                        }
                    }
                }
            }
            return PreservedAnalyses::all();
        }
    };
} // end anonymous namespace