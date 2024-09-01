cmake_minimum_required(VERSION 3.13)

project(LLVMPassPlugin)

# Locate LLVM package
find_package(LLVM REQUIRED CONFIG)

# Ensure C++11 or later is used
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM's include and link directories
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add subdirectories
add_subdirectory(passes)
add_subdirectory(test)
