set(LLVM_LINK_COMPONENTS
  Core
  Support
  Analysis
  Passes
)

add_library(LLVMPassPlugin MODULE 
  # HelloWorld.cpp
  # PointerDereference.cpp
  # StaticVarAccess.cpp
  # MemoryAlloc.cpp
  # ExternalCall.cpp
  CallGraph.cpp
  Plugin.cpp
)

llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})
target_link_libraries(LLVMPassPlugin ${llvm_libs})

# The shared library extension is not .so on Windows.
if (WIN32 OR CYGWIN)
  set(LLVMPassPlugin_name "LLVMPassPlugin${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
  set(LLVMPassPlugin_name "LLVMPassPlugin.so")
endif()

# Ensure all LLVM symbols are available
if(NOT LLVM_ENABLE_RTTI)
  set_target_properties(LLVMPassPlugin PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
  )
endif()


# Place the output in the bin directory for convenience
set_target_properties(LLVMPassPlugin PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
