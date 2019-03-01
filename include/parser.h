//
// Created by lin on 19-2-28.
//

#ifndef TINA_LANG_PARSER_H
#define TINA_LANG_PARSER_H

#include "ast.h"

// 解析表达式 声明
static ExprAst *ParseExpression();

// 解析函数原型 声明
static PrototypeAst *ParsePrototype();

// 解析括号运算符 声明
static ExprAst *ParseParenExpr();

// 数值常量语法树 声明
static ExprAst *ParseNumberExpr();

// 解析函数定义 声明
static FunctionAst *ParseDefinition();

// 辅助函数
// 简单提供一个 token 缓冲区
// CurTok 是当前语法解析器正在寻找的token
// getNextToken 是读取当前语法分析器的token，并且根据语法分析器获得结果更新CurTok
static int CurTok;
static double NumVal;
static std::string IdentifierStr;

enum Token {
    tok_eof = -1,
    // 命令
            tok_def = -2, tok_extern = -3,
    // 主要的标识符
            tok_identifier = -4, tok_number = -5
};

static int gettok() {
    return -1;
}

static int getNextToken() {
    return CurTok = gettok();
}

// 用于报错的辅助函数
ExprAst *Error(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

PrototypeAst *ErrorP(const char *str) {
    Error(str);
    return nullptr;
}

FunctionAst *ErrorF(const char *str) {
    Error(str);
    return nullptr;
}

// 数值常量语法树
static ExprAst *ParseNumberExpr() {
    ExprAst *Result = new NumberExprAst(NumVal);
    getNextToken();
    return Result;
}


// 解析括号运算符
// 该函数的特点:
// 1. 它展示了 Error 函数的用法，将错误信息输出到控制台，同时返回一个nullptr
// 2. 递归的调用了 ParseExpression (很快我们就会看到ParseExpression还会反过来调用ParseParenExpr)
// 在解析的过程中，丢弃了括号，因为当语法解析器构造完AST之后，括号就没用了.
static ExprAst *ParseParenExpr() {
    getNextToken(); // 消耗 (
    ExprAst *V = ParseExpression();

    if (!V) {
        return nullptr;
    }

    if (CurTok != ')') {
        return Error("expected ')'");
    }
    getNextToken();
    return V;
}

// 处理变量的引用和函数的调用
// 此处采用预读的风格来试探当前标识符的类型，来判断他是不是变量还是函数调用
static ExprAst *ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // 获取函数的标识符

    if (CurTok != '(') {
        // 如果不是 ( ，那么为简单的函数声明
        return new VariableExprAst(IdName);
    }

    // 调用
    getNextToken(); // 丢掉 (
    std::vector<ExprAst *> Args;
    if (CurTok != ')') {
        while (true) {
            ExprAst *Arg = ParseExpression();
            if (!Arg) {
                return nullptr;
            }
            Args.push_back(Arg);

            if (CurTok == ')') {
                break;
            }

            if (CurTok != ',') {
                return Error("Expected ')' or ',' in argument list");
            }
            // 丢掉 ,
            getNextToken();
        }
    }

}

// 辅助函数，解析主表达式入口
static ExprAst *ParsePrimary() {
    switch (CurTok) {
        default:
            return Error("unknown token where expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}

// 解析二元表达式

// 存储二元运算符的优先级
static std::map<char, int> BinopPrecedence;

// 获取二元运算符的优先级
static int GetTokPrecedence() {
    if (!isascii(CurTok)) {
        return -1;
    }
    // 确保为已声明的二元运算符
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) {
        return -1;
    }
    return TokPrec;
}

// 初始化二元运算符的优先级
// 1 是对小的优先级，数值越大，优先级越高
static void InitTokPrecedence() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
}

// 解析有序对列表
// RHS -> right hand side
static ExprAst *ParseBinOpRHS(int ExprPrec, ExprAst *LHS) {
    while (true) {
        // 如果是二元操作符，找出他的优先级
        int TokPrec = GetTokPrecedence();

        //
        if (TokPrec < ExprPrec) {
            return LHS;
        }

        int BinOp = CurTok;
        getNextToken(); // 丢掉二元操作符

        // 解析表达式
        ExprAst *RHS = ParsePrimary();
        if (!RHS) {
            return nullptr;
        }

        // 表达式的左侧与RHS的序列已经解析完毕
        // 考虑表达式的次序：
        // 1.(a + b) binop unparsed
        // 2.a + (b binop unparsed)
        // 预读 binop
        // 查出优先级
        // 与现在binop(本例中为 "+")的优先级相比较
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, RHS);
            if (RHS == nullptr) {
                return nullptr;
            }
        }
        // 合并 LHS/RHS
        LHS = new BinaryExprAst(BinOp, LHS, RHS);

    }
}

// 解析表达式
static ExprAst *ParseExpression() {
    ExprAst *LHS = ParsePrimary();
    if (!LHS) {
        return nullptr;
    }
    return ParseBinOpRHS(0, LHS);
}

// 解析函数声明
static PrototypeAst *ParsePrototype() {
    if (CurTok != tok_identifier) {
        return ErrorP("Expected function name in prototype");
    }

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(') {
        return ErrorP("Expected '(' in prototype");
    }

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier) {
        ArgNames.push_back(IdentifierStr);
    }

    if (CurTok != ')') {
        return ErrorP("Expected ')' in prototype");
    }

    // success
    getNextToken(); // 丢掉 ')'

    return new PrototypeAst(FnName, ArgNames);
}

// 解析函数定义
static FunctionAst *ParseDefinition() {
    getNextToken(); // 丢掉 def 关键词

    PrototypeAst *Proto = ParsePrototype();
    if (Proto == nullptr) {
        return nullptr;
    }
    if (ExprAst *E = ParseExpression()) {
        return new FunctionAst(Proto, E);
    }
    return nullptr;
}

// 解析 extern 关键字
static PrototypeAst *ParseExtern() {
    getNextToken(); // 丢掉 extern
    return ParsePrototype();
}

// 允许用户在顶层输入任意表达式并求值
// 该特性是通过一个特殊的匿名零元函数（没有任何参数的函数）实现的。
// 所有顶层的表达式定义在该函数内
static FunctionAst *ParseTopLevelExpr() {
    if (ExprAst *E = ParseExpression()) {
        // 创建匿名的原型
        PrototypeAst *Proto = new PrototypeAst("", std::vector<std::string>());
        return new FunctionAst(Proto, E);
    }
    return nullptr;
}

#endif //TINA_LANG_PARSER_H
