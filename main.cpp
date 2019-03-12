//
// Created by lin on 19-2-27.
//
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <iostream>
#include <db.h>
#include "include/parser.h"

//static llvm::LLVMContext TheContext;
//static llvm::IRBuilder<> Builder(TheContext);
//static std::unique_ptr<llvm::Module> TheModule;
//static std::map<std::string, llvm::Value *> NameValues;

int main(int argc, char **argv) {
//    TheModule = llvm::make_unique<llvm::Module>("hello,llvm",TheContext);
//
//    auto msg=TheModule.get();
//    std::cout<<msg<<std::endl;
//    TheModule -> dump();

//    std::cout<<"hello world"<<std::endl;

//    start_drive();

//    ::testing::InitGoogleTest( &argc, argv );
//
//    return RUN_ALL_TESTS();
    tina::db::Engine *engine = new tina::db::TinaEngine();
    engine->start();
    engine=nullptr;
    delete engine;
    return 0;
}
