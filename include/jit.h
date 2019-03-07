//
// Created by lin on 19-3-6.
//

#ifndef TINA_LANG_JIT_H
#define TINA_LANG_JIT_H
#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
//#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>

namespace tina{
    namespace orc{
        class Jit{
        private:
//            llvm::orc::ExecutionSession ES;
            llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;
            llvm::orc::IRCompileLayer CompileLayer;
            llvm::DataLayout DL;
//            llvm::MangleAndInterner Mangle;
//            llvm::ThreadSafeContext Ctx;
//        public:
//            Jit(llvm::JITTargetMachineBuilder JTMB,llvm::DataLayout DL):ObjectLayer(ES,[](){return llvm::make_unique<llvm::SectionMemoryManager>();}),
//                                                                        CompileLayer(ES)
        };
    }
}
#endif //TINA_LANG_JIT_H
