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

#endif //TINA_LANG_PARSER_H
