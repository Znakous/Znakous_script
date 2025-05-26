#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <charconv>
#include <stdexcept>
#include <exception>

#include "usings.h"
#include "object.h"
#include "ast/ast.h"

auto stoi_view(std::string_view s) {
    int ans = 0;
    if (std::from_chars(s.data(), s.data() + s.size(), ans).ec
            == std::errc::invalid_argument) {
        throw std::invalid_argument("invalid stoi_view argument");
    } 
    return ans;
}

struct ExprVisitor{
    virtual ~ExprVisitor() = default;
    virtual ptr<Object> visit(IntLiteralExpression& expr) = 0;
    virtual ptr<Object> visit(StringLiteralExpression& expr) = 0;
    virtual ptr<Object> visit(IdentifierExpression& expr) = 0;
    virtual ptr<Object> visit(PrefixExpression& expr) = 0;
};

struct Evaluator : ExprVisitor {
    auto Eval(ptr<Node>& expr) {
        return visit()
    }
    ptr<Object> visit(IntLiteralExpression& expr) {
        auto ret = make_ptr<Integer>();
        ret->value = stoi_view(expr.literal.value.value());
        return ret;
    }
    ptr<Object> visit(StringLiteralExpression& expr) {
        auto ret = make_ptr<String>();
        ret->value = expr.literal.value.value();
        return ret;
    }
    ptr<Object> visit(PrefixExpression& expr) {
        if (expr.prefix_oper == TokenType::minus) {
            return -Eval()
        }
    }
};