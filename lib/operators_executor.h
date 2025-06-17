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
        #define ADD_VISITOR(oper, visitor) visitors.emplace(TokenType::oper, std::make_unique<visitor>(log))
        ADD_VISITOR(plus, AddVisitor);
        ADD_VISITOR(minus, SubVisitor);
        ADD_VISITOR(multiply, MultVisitor);
        ADD_VISITOR(divide, DivideVisitor);
        ADD_VISITOR(eq, EqVisitor);
        ADD_VISITOR(neq, NeqVisitor);
        ADD_VISITOR(less, LessVisitor);
        ADD_VISITOR(greater, GreaterVisitor);
        ADD_VISITOR(leq, LeqVisitor);
        ADD_VISITOR(geq, GeqVisitor);
        ADD_VISITOR(mod, ModVisitor);
        ADD_VISITOR(power, PowerVisitor);
        ADD_VISITOR(bit_and, BitOperVisitor<BitOper::and_>);
        ADD_VISITOR(bit_or, BitOperVisitor<BitOper::or_>);
        ADD_VISITOR(bit_xor, BitOperVisitor<BitOper::xor_>);
        #undef ADD_VISITOR
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
        #define ADD_VISITOR(oper, visitor) visitors.emplace(TokenType::oper, std::make_unique<visitor>(log))
        ADD_VISITOR(minus, UnaryMinusVisitor);
        #undef ADD_VISITOR
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
