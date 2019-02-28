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
static std::string IdentifierStr;

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
    if (CurTok == '(') {
        ExprAst *parenExpr = ParseExpression();
    }
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
//        case tok_identifier:
//            return ParseIdentifierExpr();
//        case tok_number:
//            return ParseNumberExpr();
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

#endif //TINA_LANG_PARSER_H
