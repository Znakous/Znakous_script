#pragma once

#include <unordered_map>
#include <iostream>
#include <object.h>
#include <token.h>
#include <logger.h>
#include <get_name.h>
#include <operators/operators.h>
#include <build/binary_opers_all.h>
#include <build/unary_opers_all.h>


struct DummyInvoker {
    template<typename T, typename... Args>
    auto operator()(T&& obj, Args&&... args) {
        return obj(args...);    
    }
};


using var_type_binary = std::variant<binary_opers>;
using var_type_unary = std::variant<unary_opers>;

struct BinaryOperatorExecutor {
    BinaryOperatorExecutor(std::shared_ptr<logging::Logger> log)
     : logger_(log)
     {
        auto names = GetNames<TokenType, var_type_binary, TokenTag>().names;
        for (auto& func : names) {
            // auto var = func.value;
            visitors.emplace(func.name, func.value);
        }
    }
    
        
    Object Execute(TokenType oper, Object& a, Object& b) {
        logger_->log("Executing binary operator: ", (int)oper);
        logger_->log("Operand types: ", typeid(a).name(), " and ", typeid(b).name());
        auto result = std::visit(DummyInvoker{}, visitors[oper], static_cast<ObjVar>(a), static_cast<ObjVar>(b));
        logger_->log("Binary operation result type: ", typeid(result).name());
        return result;
    }
    std::unordered_map<TokenType, var_type_binary> visitors;
    std::shared_ptr<logging::Logger> logger_;
};



struct UnaryOperatorExecutor {
    UnaryOperatorExecutor(std::shared_ptr<logging::Logger> log)
    : logger_(log)
    {
        auto names = GetNames<TokenType, var_type_unary, TokenTag>().names;
        for (auto& func : names) {
            // auto var = func.value;
            visitors.emplace(func.name, func.value);
        }
    }

    Object Execute(TokenType oper, Object& a) {
        logger_->log("Executing unary operator: ", (int)oper);
        logger_->log("Operand type: ", typeid(a).name());
        auto result = std::visit(DummyInvoker{}, visitors[oper], static_cast<ObjVar>(a));
        logger_->log("Unary operation result type: ", typeid(result).name());
        return result;
    }
    std::unordered_map<TokenType, var_type_unary> visitors;
    std::shared_ptr<logging::Logger> logger_;
};
