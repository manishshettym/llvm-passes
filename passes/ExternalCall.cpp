#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"

using namespace llvm;

namespace
{
    class ExternalFunctionCallDetector : public PassInfoMixin<ExternalFunctionCallDetector>
    {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &)
        {
            for (auto &I : instructions(F))
            {
                // Detect direct calls to external functions
                if (auto *CI = dyn_cast<CallInst>(&I))
                {
                    if (Function *Callee = CI->getCalledFunction())
                    {
                        // External function: declared but not defined in this module
                        if (Callee->isDeclaration() && !Callee->isIntrinsic())
                        {

                            if (Callee->getName() == "malloc" || Callee->getName() == "free")
                            {
                                continue;
                            }

                            errs() << "External function call detected: "
                                   << Callee->getName() << " in function " << F.getName() << "\n";
                        }
                    }
                    // Indirect call: function pointer call
                    else if (CI->isIndirectCall())
                    {
                        errs() << "Indirect function call (via function pointer) detected in function "
                               << F.getName() << "\n";
                    }
                }
            }
            return PreservedAnalyses::all();
        }
    };
} // end anonymous namespace