# FunctionPassManager 

## FunctionPassManager 如何找到
```cpp
#include <llvm/IR/LegacyPassManager.h>

llvm::make_unique<llvm::legacy::FunctionPassManager> TheFPM=llvm::make_unique<llvm::legacy::FunctionPassManager>(TheModule);
```