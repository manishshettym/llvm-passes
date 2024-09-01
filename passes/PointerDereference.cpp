// RawPointerDereferenceDetector.cpp
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
    class RawPointerDereferenceDetector : public PassInfoMixin<RawPointerDereferenceDetector>
    {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &)
        {
            for (BasicBlock &BB : F)
            {
                for (Instruction &I : BB)
                {
                    // Detect pointer arithmetic (e.g., ptr++, ptr + n)
                    if (auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                        errs() << "Pointer arithmetic detected in function "
                               << F.getName() << ": " << *GEP << "\n";
                    }

                    // Detect casts to pointers (e.g., (int *) ptr)
                    if (auto *CI = dyn_cast<CastInst>(&I))
                    {
                        if (CI->getDestTy()->isPointerTy())
                        {
                            errs() << "Pointer cast detected in function "
                                   << F.getName() << ": " << *CI << "\n";
                        }
                    }
                }
            }
            return PreservedAnalyses::all();
        }
    };
} // end anonymous namespace