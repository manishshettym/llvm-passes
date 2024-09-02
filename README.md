# crust-llvm
LLVM passes do detect potential sources of safety violations in c2rust


## Building

To build the project, you need to have LLVM installed. You can install it using the following command:

```bash
sudo apt-get install llvm-10-dev
```

Then, you can build the project using the following command:

```bash
sh build.sh
```

## Running

To run any of the passes, you can use the following command:

```bash
sh run.sh <pass-name>
```

By default, the pass will be run on the file `/test/test.c`. You can change the file by modifying the `run.sh` script.

## Passes

The following passes are available:
- `hello-world`: A simple pass that prints every function in the module.
- `raw-pointer-deref`: A pass that detects raw pointer usage in the code.
- `static-var-access`: A pass that detects read/write to mutable global (static) or external variables.
- `memory-alloc`: A pass that detects memory allocation and deallocation in the code.
- `external-call`: A pass that detects external function calls (other than one defined in the same program, malloc, and free). Also detects calls made via function pointers.
- `unsafe-cast`: A pass that detects unsafe casts in the code.