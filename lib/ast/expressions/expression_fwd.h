#pragma once


template<size_t Level>
struct Operator;

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

struct FunctionalExpression;

using Expression = std::variant<ptr<FunctionalExpression>, ptr<ExpressionImpl<operators_levels>>>;