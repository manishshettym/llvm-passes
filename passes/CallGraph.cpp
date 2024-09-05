#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/TypeBasedAliasAnalysis.h"
#include "llvm/Analysis/ScopedNoAliasAA.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include "llvm/IR/PassManager.h"

using namespace llvm;

namespace
{

    struct FunctionInfo
    {
        unsigned CallCount = 0;
        bool IsRecursive = false;
        std::vector<std::string> DirectCalls;
        std::vector<std::string> IndirectCalls;
    };

    void writeCallGraphToJSON(const CallGraph &CG, const std::map<const Function *, FunctionInfo> &FuncInfoMap, StringRef Filename)
    {
        std::error_code EC;
        raw_fd_ostream OS(Filename, EC, sys::fs::OF_Text);
        if (EC)
        {
            errs() << "Error opening file " << Filename << ": " << EC.message() << "\n";
            return;
        }

        json::Object Root;
        json::Array Nodes;

        for (const auto &Entry : CG)
        {
            const Function *F = Entry.first;
            if (!F)
                continue; // Skip external node

            json::Object Node;
            Node["name"] = F->getName();
            Node["variadic"] = F->isVarArg();

            const auto &FuncInfo = FuncInfoMap.at(F);
            Node["call_count"] = FuncInfo.CallCount;
            Node["is_recursive"] = FuncInfo.IsRecursive;

            json::Array DirectCalls;
            for (const auto &Callee : FuncInfo.DirectCalls)
            {
                DirectCalls.push_back(Callee);
            }
            Node["direct_calls"] = std::move(DirectCalls);

            json::Array IndirectCalls;
            for (const auto &Callee : FuncInfo.IndirectCalls)
            {
                IndirectCalls.push_back(Callee);
            }
            Node["indirect_calls"] = std::move(IndirectCalls);

            Nodes.push_back(std::move(Node));
        }

        Root["nodes"] = std::move(Nodes);
        json::Value JsonValue(std::move(Root));
        OS << formatv("{0:2}", JsonValue);
        OS.close();
    }

    class CallGraphPass : public PassInfoMixin<CallGraphPass>
    {
    public:
        PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
        {
            CallGraph CG(M);
            std::map<const Function *, FunctionInfo> FuncInfoMap;

            auto &FAM =
                MAM.getResult<FunctionAnalysisManagerModuleProxy>(M).getManager();

            for (Function &F : M)
            {
                if (F.isDeclaration())
                    continue;

                auto &AA = FAM.getResult<AAManager>(F);

                FunctionInfo &FI = FuncInfoMap[&F];

                for (BasicBlock &BB : F)
                {
                    for (Instruction &I : BB)
                    {
                        if (auto *CI = dyn_cast<CallInst>(&I))
                        {
                            handleCallInstruction(CI, CG, FI, F, AA, M, FuncInfoMap);
                        }
                    }
                }
            }

            // Write the call graph to a JSON file
            writeCallGraphToJSON(CG, FuncInfoMap, "callgraph.json");

            return PreservedAnalyses::all();
        }

    private:
        void handleCallInstruction(CallInst *CI, CallGraph &CG, FunctionInfo &FI, Function &F,
                                   AAResults &AA, Module &M, std::map<const Function *, FunctionInfo> &FuncInfoMap)
        {
            Function *Callee = CI->getCalledFunction();
            if (Callee)
            {
                // Direct call
                handleDirectCall(CI, Callee, CG, FI, F, FuncInfoMap);
            }
            else
            {
                // Indirect call
                handleIndirectCall(CI, CG, FI, F, AA, M, FuncInfoMap);
            }
        }

        void handleDirectCall(CallInst *CI, Function *Callee, CallGraph &CG, FunctionInfo &FI, Function &F,
                              std::map<const Function *, FunctionInfo> &FuncInfoMap)
        {
            CG[&F]->addCalledFunction(CI, CG[Callee]);
            FI.DirectCalls.push_back(Callee->getName().str());
            FuncInfoMap[Callee].CallCount++;
            if (Callee == &F)
            {
                FI.IsRecursive = true;
            }
        }

        void handleIndirectCall(CallInst *CI, CallGraph &CG, FunctionInfo &FI, Function &F,
                                AAResults &AA, Module &M, std::map<const Function *, FunctionInfo> &FuncInfoMap)
        {
            Value *CalledValue = CI->getCalledOperand()->stripPointerCasts();

            // Use alias analysis to find possible targets
            std::vector<Function *> PossibleTargets;
            for (Function &PossibleCallee : M)
            {
                if (AA.isMustAlias(&PossibleCallee, CalledValue))
                {
                    PossibleTargets.clear();
                    PossibleTargets.push_back(&PossibleCallee);
                    break;
                }
                else if (!AA.isNoAlias(&PossibleCallee, CalledValue))
                {
                    // may alias, but only include if the function type matches
                    if (PossibleCallee.getFunctionType() == CI->getFunctionType())
                    {
                        PossibleTargets.push_back(&PossibleCallee);
                    }
                }
            }

            if (PossibleTargets.empty())
            {
                FI.IndirectCalls.push_back("unknown");
            }
            else
            {
                for (Function *Target : PossibleTargets)
                {
                    CG[&F]->addCalledFunction(CI, CG[Target]);
                    FI.IndirectCalls.push_back(Target->getName().str());
                    FuncInfoMap[Target].CallCount++;
                }
            }
        }
    };
} // end anonymous namespace