cmake_minimum_required(VERSION 3.9)
project(tina-lang)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


set(CMAKE_CXX_STANDARD 11)

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


message(STATUS "工程源代码路径: ${PROJECT_SOURCE_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(tina-lang main.cpp include/ast.h include/parser.h include/jit.h test/tina_test.cpp include/db.h lib/db.cpp include/common.h)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs analysis asmparser binaryformat support core irreader tablegen xray executionengine interpreter
        object objectyaml mc mcjit bitwriter x86codegen target orcjit passes linker option nativecodegen instcombine runtimedyld scalaropts transformutils native)

# Link against LLVM libraries
target_link_libraries(tina-lang ${llvm_libs})

