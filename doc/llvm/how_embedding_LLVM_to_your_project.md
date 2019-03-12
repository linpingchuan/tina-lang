# 如何将 LLVM 嵌套到你的项目中去

## 安装llvm
```shell
cmake ../llvm/ -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_BUILD_TESTS=False 
``` 


> 本项目使用 cmake 构建项目

> IDE 为 clion

> CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.9)
project(clang_demo)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 17)

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(clang_demo main.cpp toy.cpp hello_world.cpp)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(clang_demo ${llvm_libs})
```

### 不使用 cmake 构建项目，可执行以下命令
```shell
clang++ -g hello_world.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o hello_world
```

获取 LLVM 配置信息 
```shell
llvm-config --cxxflags --ldflags --system-libs --libs core
```


