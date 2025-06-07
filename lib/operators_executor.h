#pragma once

#include <unordered_map>
#include <iostream>

#include "object.h"
#include "token.h"
#include "visitor.h"
#include "logger.h"
// #include "std_functions.h"



struct BinaryOperatorExecutor {
    BinaryOperatorExecutor(std::shared_ptr<logging::Logger> log)
     : logger_(log)
    {
        visitors.emplace(TokenType::plus, std::make_unique<AddVisitor>(log));
        visitors.emplace(TokenType::minus, std::make_unique<SubVisitor>(log));
        visitors.emplace(TokenType::multiply, std::make_unique<MultVisitor>(log));
        visitors.emplace(TokenType::divide, std::make_unique<DivideVisitor>(log));
    }

    Object Execute(TokenType oper, Object& a, Object& b) {
        logger_->log("Executing binary operator: ", (int)oper);
        logger_->log("Operand types: ", typeid(a).name(), " and ", typeid(b).name());
        auto result = (*visitors[oper])(a, b);
        logger_->log("Binary operation result type: ", typeid(result).name());
        return result;
    }

    std::unordered_map<TokenType, std::unique_ptr<BinaryOperVisitor>> visitors;
    std::shared_ptr<logging::Logger> logger_;
};

struct UnaryOperatorExecutor {
    UnaryOperatorExecutor(std::shared_ptr<logging::Logger> log)
     : logger_(log)
    {
        visitors.emplace(TokenType::minus, std::make_unique<UnaryMinusVisitor>(log));
    }

    Object Execute(TokenType oper, Object& a) {
        logger_->log("Executing unary operator: ", (int)oper);
        logger_->log("Operand type: ", typeid(a).name());
        auto result = (*visitors[oper])(a);
        logger_->log("Unary operation result type: ", typeid(result).name());
        return result;
    }

    std::unordered_map<TokenType, std::unique_ptr<UnaryOperVisitor>> visitors;
    std::shared_ptr<logging::Logger> logger_;
};
