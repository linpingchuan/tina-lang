//
// Created by lin on 19-2-28.
//

#ifndef TINA_LANG_PARSER_H
#define TINA_LANG_PARSER_H

#include "ast.h"

static int gettok() {
    return -1;
}

// 辅助函数
// 简单提供一个 token 缓冲区
// CurTok 是当前语法解析器正在寻找的token
// getNextToken 是读取当前语法分析器的token，并且根据语法分析器获得结果更新CurTok
static int CurTok;
static double NumVal;

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

// 解析表达式
static ExprAst *ParseExpression() {
    return nullptr;
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

#endif //TINA_LANG_PARSER_H
