#pragma once

#include <variant>
#include <usings.h>

template<size_t Level>
struct Operator;

// level 0 Expressions

struct IntLiteralExpression;

struct StringLiteralExpression;

struct NullLiteralExpression;

struct IdentifierExpression;

struct FunctionCallExpression;

struct StdFuncCallExpression;

struct PrefixExpression;

struct ScopedExpression;

struct ArrayExpression;

struct ArrayAccessExpression;

struct ArraySliceExpression;

// level-building

template<size_t Level>
struct ExpressionImpl;

template<>
struct ExpressionImpl<0>;

struct FunctionalExpression;

using Expression = std::variant<ptr<FunctionalExpression>, 
                ptr<ExpressionImpl<operators_levels>>>;