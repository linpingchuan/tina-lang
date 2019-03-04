//
// Created by lin on 19-2-28.
//

#ifndef TINA_LANG_AST_H
#define TINA_LANG_AST_H

#include <string>
#include <vector>
#include <llvm-c/Core.h>

// LLVM 用于存放代码片段中所有函数与全局变量的结构
// 相当于 LLVM 的 IR 的顶层容器
static llvm::Module *TheModule;

static llvm::LLVMContext TheContext;
// 简化 LLVM 指令生成的辅助对象
// IRBuilder模板的实例可用于跟踪当前插入指令的位置，同时还带有用于生成新指令的方法
static llvm::IRBuilder<> Builder(TheContext);
// 该映射表用于记录定义于当前作用域内的变量与相对应的 LLVM 表示（就是代码的符号表）
static std::map<std::string, llvm::Value *> NameValues;

// 抽象语法树
class ExprAst {
public:
    virtual ~ExprAst() = default;

    // Codegen 方法负责生成该该类型 AST 节点的 IR 代码及其他必要信息
    // 生成内容以 llvm::Value 对象的形式返回。
    // LLVM用 "Value" 类表示 ”静态一次性赋值 SSA(static single assignment)寄存器“
    // SSA 值最为突出的特点就在于”固定不变“： SSA值经由对应指令运算得出后便固定下来，直到该指令再次执行之前都不可修改
    // visitor 模式实现代码生成
    virtual llvm::Value *Codegen()=0;
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

llvm::Value *VariableExprAst::Codegen() {
    return nullptr;
}
// 运算符表达式
class BinaryExprAst : public ExprAst {
    char Op;
    ExprAst *LHS, *RHS;
public:
    BinaryExprAst(char op, ExprAst *lhs, ExprAst *rhs) : Op(op), LHS(lhs), RHS(rhs) {}

    llvm::Value *Codegen();
};

llvm::Value *BinaryExprAst::Codegen() {
    return nullptr;
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
    return nullptr;
}
// 函数接口
class PrototypeAst {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAst(const std::string &name, const std::vector<std::string> &args) : Name(name), Args(args) {}
};

// 函数本身
class FunctionAst {
    PrototypeAst *Proto;
    ExprAst *Body;
public:
    FunctionAst(PrototypeAst *proto, ExprAst *body) : Proto(proto), Body(body) {}
};

#endif //TINA_LANG_AST_H
