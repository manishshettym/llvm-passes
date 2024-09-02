#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
    class StaticVarAccessDetector : public PassInfoMixin<StaticVarAccessDetector>
    {
    public:
        PreservedAnalyses run(Module &M, ModuleAnalysisManager &)
        {
            for (Function &F : M.functions())
            {
                analyzeFunction(F);
            }
            return PreservedAnalyses::all();
        }

        void analyzeFunction(Function &F)
        {
            for (BasicBlock &BB : F)
            {
                for (Instruction &I : BB)
                {
                    if (StoreInst *SI = dyn_cast<StoreInst>(&I))
                    {
                        Value *Ptr = SI->getPointerOperand();
                        if (GlobalVariable *GV = dyn_cast<GlobalVariable>(Ptr))
                        {
                            errs() << "Write to global/static/external variable: "
                                   << GV->getName() << " in function " << F.getName() << "\n";
                        }
                    }
                    else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
                    {
                        Value *Ptr = LI->getPointerOperand();
                        if (GlobalVariable *GV = dyn_cast<GlobalVariable>(Ptr))
                        {
                            errs() << "Read from global/static/external variable: "
                                   << GV->getName() << " in function " << F.getName() << "\n";
                        }
                    }
                }
            }
        }
    };
} // end anonymous namespace