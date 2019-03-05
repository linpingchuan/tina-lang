//
// Created by lin on 19-2-28.
//

#ifndef TINA_LANG_AST_H
#define TINA_LANG_AST_H

#include <string>
#include <vector>
#include <llvm-c/Core.h>
#include "parser.h"

class FunctionAst;

extern llvm::Value *ErrorV(const char *Str);
extern FunctionAst *ErrorF(const char *Str);

// LLVM 用于存放代码片段中所有函数与全局变量的结构
// 相当于 LLVM 的 IR 的顶层容器
static llvm::Module *TheModule;

static llvm::LLVMContext TheContext;
// 简化 LLVM 指令生成的辅助对象
// IRBuilder模板的实例可用于跟踪当前插入指令的位置，同时还带有用于生成新指令的方法
static llvm::IRBuilder<> Builder(TheContext);
// 该映射表用于记录定义于当前作用域内的变量与相对应的 LLVM 表示（就是代码的符号表）
static std::map<std::string, llvm::Value *> NamedValues;

// 抽象语法树
class ExprAst {
public:
    virtual ~ExprAst() = default;

    // Codegen 方法负责生成该该类型 AST 节点的 IR 代码及其他必要信息
    // 生成内容以 llvm::Value 对象的形式返回。
    // LLVM用 "Value" 类表示 ”静态一次性赋值 SSA(static single assignment)寄存器“
    // SSA 值最为突出的特点就在于”固定不变“： SSA值经由对应指令运算得出后便固定下来，直到该指令再次执行之前都不可修改
    // visitor 模式实现代码生成
    virtual llvm::Value *Codegen()= 0;
};

// 数字语法树
class NumberExprAst : public ExprAst {
    double Val;
public:
    explicit NumberExprAst(double val) : Val(val) {}

    llvm::Value *Codegen();
};

// LLVM IR中的数值常量由ConstantFP类表示
llvm::Value *NumberExprAst::Codegen() {
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
}

// 变量语法树
class VariableExprAst : public ExprAst {
    std::string Name;
public:
    explicit VariableExprAst(const std::string &name) : Name(name) {}

    llvm::Value *Codegen();
};

// 在 LLVM IR内部，常量都只有一份，并且是共享的
llvm::Value *VariableExprAst::Codegen() {
    llvm::Value *V = NameValues[Name];
    return V ? V : ErrorV("Unknown variable name");
}

// 运算符表达式
class BinaryExprAst : public ExprAst {
    char Op;
    ExprAst *LHS, *RHS;
public:
    BinaryExprAst(char op, ExprAst *lhs, ExprAst *rhs) : Op(op), LHS(lhs), RHS(rhs) {}

    llvm::Value *Codegen();
};

// uitofp -> 输入的整数视作无符号数，并将之转为浮点数
llvm::Value *BinaryExprAst::Codegen() {
    llvm::Value *L = LHS->Codegen();
    llvm::Value *R = RHS->Codegen();
    if (L == nullptr || R == nullptr) {
        return nullptr;
    }
    switch (Op) {
        case '+':
            return Builder.CreateFAdd(L, R, "cmptmp");
        case '-':
            return Builder.CreateFSub(L, R, "subtmp");
        case '*':
            return Builder.CreateFMul(L, R, "multmp");
        case '<':
            L = Builder.CreateFCmpULT(L, R, "cmptmp");
            return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
        default:
            return ErrorV("invalid binary operator");
    }
}

// 函数名和用作参数的表达式
class CallExprAst : public ExprAst {
    std::string Callee;
    std::vector<ExprAst *> Args;
public:
    CallExprAst(const std::string &callee, std::vector<ExprAst *> &args) : Callee(callee), Args(args) {}

    llvm::Value *Codegen();
};

llvm::Value *CallExprAst::Codegen() {
    // 通过名字查询全局表
    llvm::Function *CalleF = TheModule->getFunction(Callee);
    if (CalleF == nullptr) {
        return ErrorV("Unknown function referenced");
    }
    // 如果参数匹配错误
    if (CalleF->arg_size() != Args.size()) {
        return ErrorV("Incorrect # arguments passed");
    }
    std::vector<llvm::Value *> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->Codegen());
        if (ArgsV.back() == nullptr) {
            return nullptr;
        }
    }
    return Builder.CreateCall(CalleF, ArgsV, "calltmp");
}

// 函数原型接口
class PrototypeAst {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAst(const std::string &name, const std::vector<std::string> &args) : Name(name), Args(args) {}

    llvm::Function *Codegen();
};

// Externallinkage 表示该函数可能定义于当前模块之外，可以被当前模块之外的函数调用
// Name 用户指定的函数名
// 函数定义于 “TheModule" 内，函数名自然也注册在 ”TheModule" 的符号表内
llvm::Function *PrototypeAst::Codegen() {
    std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(TheContext));
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext), Doubles, false);
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule);
    // 在处理名称冲突时，Module的符号表与Function的符号表类似：
    // 在模块中添加新函数，如果发现函数名与符号表中现有的名称重复，
    // 新函数会被默默的重命名
    if (F->getName().str() != Name) {
        F->eraseFromParent();
        F = TheModule->getFunction(Name);

        // 只能是前导声明
        // 如果已经定义函数体，直接返回
        if(!F->empty()){
            ErrorF("redefinition of function");
            return nullptr;
        }

        // 如果参数不同，直接返回
        if(F->arg_size()!=Args.size()){
            ErrorF("redefinition of function with different # args");
            return nullptr;
        }

        // 遍历函数原型的所有参数
        // 为这些 LLVM Argument 对象逐一设置参数名
        // 并将这些参数注册到 NamedValues 映射表内，以备 AST 节点类 VariableExprAst 稍后使用。

        unsigned Idx=0;
        for(llvm::Function::arg_iterator AI=F->arg_begin();Idx!=Args.size();++AI,++Idx){
            AI->setName(Args[Idx]);
            //
            NamedValues[Args[Idx]]=AI;
        }
    }
}

// 函数本身
class FunctionAst {
    PrototypeAst *Proto;
    ExprAst *Body;
public:
    FunctionAst(PrototypeAst *proto, ExprAst *body) : Proto(proto), Body(body) {}
    llvm::Function *Codegen();
};

llvm::Function *FunctionAst::Codegen(){
    NamedValues.clear();

    llvm::Function *TheFunction=Proto->Codegen();
    if(TheFunction == nullptr){
        return nullptr;
    }

    llvm::BasicBlock *BB=llvm::BasicBlock::Create(TheContext,"entry",TheFunction);
    Builder.SetInsertPoint(BB);
    // TODO
    if(llvm::Value *RetVal=Body->Codegen()){
        Builder.CreateRet(RetVal);
//        verifyFunction(*TheFunction);
        return TheFunction;
    }
}

#endif //TINA_LANG_AST_H
