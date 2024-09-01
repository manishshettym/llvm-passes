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

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
  return {
      LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
      [](PassBuilder &PB)
      {
        PB.registerPipelineParsingCallback(
            [](StringRef Name, FunctionPassManager &FPM, ArrayRef<PassBuilder::PipelineElement>)
            {
              if (Name == "hello-world")
              {
                FPM.addPass(HelloWorldPass());
                return true;
              }
              return false;
            });
      }};
}