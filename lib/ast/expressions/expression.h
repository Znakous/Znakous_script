#pragma once

#include <variant>
#include <vector>
#include <algorithm>
#include <optional>

#include "../../../lib/token.h"
#include "../../../lib/usings.h"
#include "../../../lib/environment_fwd.h"
#include "expression_fwd.h"
#include "../statements/statement_fwd.h"


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
    ptr<IntLiteralExpression>, ptr<IdentifierExpression>, ptr<ScopedExpression>,
    ptr<ArrayExpression>, ptr<ArrayAccessExpression>
    > value;
};



struct FunctionalExpression {
    std::vector<Statement> body;
    std::vector<std::string_view> arguments;
    ptr<Environment> closure_env;
};


struct ArrayExpression {
    std::vector<Expression> elements;
};

struct ArrayAccessExpression {
    std::string_view array;
    std::vector<Expression> indices;
};

