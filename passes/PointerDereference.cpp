#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
    class RawPointerDereferenceDetector : public PassInfoMixin<RawPointerDereferenceDetector>
    {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &)
        {
            for (auto &BB : F)
            {
                for (auto &I : BB)
                {
                    // Detect direct dereferencing of pointers
                    if (auto *LI = dyn_cast<LoadInst>(&I))
                    {
                        if (LI->getPointerOperand()->getType()->isPointerTy())
                        {
                            errs() << "Direct pointer dereference (load) detected in function "
                                   << F.getName() << ": " << *LI << "\n";
                        }
                    }
                    else if (auto *SI = dyn_cast<StoreInst>(&I))
                    {
                        if (SI->getPointerOperand()->getType()->isPointerTy())
                        {
                            errs() << "Direct pointer dereference (store) detected in function "
                                   << F.getName() << ": " << *SI << "\n";
                        }
                    }

                    // Detect pointer arithmetic (e.g., ptr++, ptr + n)
                    else if (auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                        errs() << "Pointer arithmetic detected in function "
                               << F.getName() << ": " << *GEP << "\n";
                    }

                    // Detect casts to pointers (e.g., (int *) ptr)
                    else if (auto *CI = dyn_cast<CastInst>(&I))
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