#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

// Include pass headers
#include "HelloWorld.cpp"
#include "PointerDereference.cpp"
#include "StaticVarAccess.cpp"

using namespace llvm;

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "MyPasses", LLVM_VERSION_STRING,
        [](PassBuilder &PB)
        {
            // Register function passes
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

            // Register module passes
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM, ArrayRef<PassBuilder::PipelineElement>)
                {
                    if (Name == "static-var-access")
                    {
                        MPM.addPass(StaticVarAccessDetector());
                        return true;
                    }
                    return false;
                });
        }};
}