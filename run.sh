#!/bin/bash

cd "$(dirname "$0")"

CLANG=$(which clang)
OPT=$(which opt)
PASS_PLUGIN_PATH="./build/bin/libLLVMHelloWorld.so"

# Ensure we're in the project root directory
if [ ! -f $PASS_PLUGIN_PATH ]; then
    echo "Pass plugin not found. Please ensure you're in the project root and have built the project."
    exit 1
fi

$CLANG -S -emit-llvm -O1 ./test/test.c -o ./test/test.ll

$OPT -load-pass-plugin $PASS_PLUGIN_PATH -passes="hello-world" < ./test/test.ll >/dev/null 2>./test/logs.log

echo "Pass has been run. Output is in ./test/logs.log"

# cleanup
rm ./test/test.ll