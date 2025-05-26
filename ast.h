#pragma once

#include "lexer.h"

#include <variant>
#include <vector>
#include <tuple>
#include <memory>



constexpr size_t operators_levels = 3;

struct Node {
    virtual ~Node() = default;
};

template<size_t Level>
struct Operator : Node {};

struct UnaryMinus : Operator<1> {};
struct UnaryPlus : Operator<1> {};

struct FuncCall : Operator<0> {};

template<size_t Level>
struct ExpressionImpl {
    ptr<ExpressionImpl<Level - 1>> first;
    std::vector<std::pair<ptr<Operator<Level>>, ptr<ExpressionImpl<Level - 1>>>> others;
};

template<>
struct ExpressionImpl<0> {

};

using Expression = ExpressionImpl<operators_levels>;

struct LiteralExpression : ExpressionImpl<0> {};

struct IntLiteralExpression : LiteralExpression {
    Token literal;
};

struct StringLiteralExpression : LiteralExpression {
    Token literal;
};

struct FunctionalExpression : ExpressionImpl<0> {
    Token func_keyword;
    ptr<Expression> underlying;
};

struct IdentifierExpression : ExpressionImpl<0> {
    Token identifier;
};

struct FunctionCallExpression : ExpressionImpl<0> {
    ptr<IdentifierExpression> function;
    std::vector<ptr<Expression>> arguments;
};

struct PrefixExpression : ExpressionImpl<0> {
    Token prefix_oper;
    ptr<ExpressionImpl<0>> underlying;
};

struct ScopedExpression : ExpressionImpl<0> {
    Token lparen;
    ptr<Expression> underlying;
};



struct Identifier : Node {
    ptr<Token> token;
};



struct Statement : Node {};

struct ElseIfStatement : Statement {
    ptr<Expression> condition;
    ptr<Statement> underlying;
};

struct ElseStatement : Statement {
    Token token;
    ptr<Statement> underlying;
};

struct IfStatement : Statement {
    ptr<Expression> condition;
    ptr<Statement> underlying;
    std::vector<ptr<ElseIfStatement>> else_ifs;
    std::optional<ptr<ElseStatement>> else_s;
};

struct AssignStatement : Statement {
    ptr<Identifier> ident;
    ptr<Expression> expr;
};

struct Program : Node {
    std::vector<ptr<Statement>> statements;
};