#pragma once

#include <variant>
#include <vector>
#include <algorithm>


#include <token.h>
#include <usings.h>
#include <ast/statements/statement_fwd.h>
#include <ast/expressions/expression_fwd.h>



template<size_t Level>
struct Operator {
    Token token;
};

// level 0 Expressions

struct IntLiteralExpression;

struct StringLiteralExpression;

struct IdentifierExpression;

struct FunctionCallExpression;

struct PrefixExpression;

struct ScopedExpression;

// level-building

template<size_t Level>
struct ExpressionImpl;

template<>
struct ExpressionImpl<0>;


struct IntLiteralExpression {
    Token literal;
};

struct StringLiteralExpression {
    Token literal;
};

struct FunctionCallExpression {
    Token token;
    std::string_view function;
    std::vector<Expression> arguments;
};

struct IdentifierExpression {
    std::string_view name;
};

struct ScopedExpression {
    Expression underlying;
};

template<size_t Level>
struct ExpressionImpl {
    ptr<ExpressionImpl<Level - 1uz>> first;
    std::vector<std::pair<Operator<Level>, ptr<ExpressionImpl<Level-1>>>> others;
};

template<>
struct ExpressionImpl<1> { // for prefix operations only
    std::optional<Token> prefix_oper;
    ptr<ExpressionImpl<0>> underlying;
};
template<>
struct ExpressionImpl<0> {
    std::variant<ptr<FunctionCallExpression>, ptr<StringLiteralExpression>,
    ptr<IntLiteralExpression>, ptr<IdentifierExpression>, ptr<ScopedExpression>
    > value;
};



struct FunctionalExpression {
    std::vector<Statement> body;
    std::vector<std::string_view> arguments;
    // Expression return_s;
};

