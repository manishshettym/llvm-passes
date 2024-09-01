set(LLVM_LINK_COMPONENTS
  Core
  Support
)

add_library(LLVMHelloWorld MODULE HelloWorld.cpp)
llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})
target_link_libraries(LLVMHelloWorld ${llvm_libs})

# The shared library extension is not .so on Windows.
if (WIN32 OR CYGWIN)
  set(LLVMHelloWorld_name "LLVMHelloWorld${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
  set(LLVMHelloWorld_name "LLVMHelloWorld.so")
endif()

# Place the output in the bin directory for convenience
set_target_properties(LLVMHelloWorld PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
