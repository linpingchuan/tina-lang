//
// Created by lin on 19-2-28.
//

#ifndef TINA_LANG_AST_H
#define TINA_LANG_AST_H

#include <string>
#include <vector>

// 抽象语法树
class ExprAst {
public:
    virtual ~ExprAst() = default;
};

// 数字语法树
class NumberExprAst : public ExprAst {
    double Val;
public:
    explicit NumberExprAst(double val) : Val(val) {}
};

// 变量语法树
class VariableExprAst : public ExprAst {
    std::string Name;
public:
    explicit VariableExprAst(const std::string &name) : Name(name) {}
};

// 运算符表达式
class BinaryExprAst : public ExprAst {
    char Op;
    ExprAst *LHS, *RHS;
public:
    BinaryExprAst(char op, ExprAst *lhs, ExprAst *rhs) : Op(op), LHS(lhs), RHS(rhs) {}
};

// 函数名和用作参数的表达式
class CallExprAst : public ExprAst {
    std::string Callee;
    std::vector<ExprAst *> Args;
public:
    CallExprAst(const std::string &callee, std::vector<ExprAst *> &args) : Callee(callee), Args(args) {}
};

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
