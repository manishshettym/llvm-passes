#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

// Include pass headers
#include "HelloWorld.cpp"
#include "PointerDereference.cpp"

using namespace llvm;

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "MyPasses", LLVM_VERSION_STRING,
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
                    if (Name == "raw-pointer-deref")
                    {
                        FPM.addPass(RawPointerDereferenceDetector());
                        return true;
                    }
                    return false;
                });
        }};
}