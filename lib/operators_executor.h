#pragma once

#include <unordered_map>

#include "object.h"
#include "token.h"
#include "visitor.h"
// #include "std_functions.h"




struct BinaryOperatorExecutor {
    BinaryOperatorExecutor() {
        visitors.emplace(TokenType::plus, std::make_unique<AddVisitor>());
        visitors.emplace(TokenType::minus, std::make_unique<SubVisitor>());
        visitors.emplace(TokenType::multiply, std::make_unique<MultVisitor>());
    }

    Object Execute(TokenType oper, Object& a, Object& b) {
        return (*visitors[oper])(a, b);
    }

    std::unordered_map<TokenType, std::unique_ptr<BinaryOperVisitor>> visitors;
};

struct UnaryOperatorExecutor {
    UnaryOperatorExecutor() {
        visitors.emplace(TokenType::minus, std::make_unique<UnaryMinusVisitor>());
    }

    Object Execute(TokenType oper, Object& a) {
        return (*visitors[oper])(a);
    }

    std::unordered_map<TokenType, std::unique_ptr<UnaryOperVisitor>> visitors;
};
