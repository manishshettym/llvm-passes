#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
  class HelloWorldPass : public PassInfoMixin<HelloWorldPass>
  {
  public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &)
    {
      errs() << "Hello from: " << F.getName() << "\n";
      return PreservedAnalyses::all();
    }
  };
} // end anonymous namespace