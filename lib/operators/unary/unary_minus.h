#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: UnaryMinusVisitor

struct UnaryMinusVisitor : UnaryOperVisitor {
    using UnaryOperVisitor::UnaryOperVisitor;
    virtual Object operator()(Object& a) {
        logger_->log("Unary minus on type: ", typeid(a).name());
        auto result = std::visit(*this, a);
        logger_->log("Unary minus result index: ", result.index());
        return result;
    }
    Object operator()(const double& a) { 
        logger_->log("Negating double: ", a);
        return -a; 
    }
    Object operator()(const std::string& a) { 
        logger_->log("Reversing string: ", a);
        std::string result = a;
        std::reverse(result.begin(), result.end());
        logger_->log("Reversed string: ", result);
        return result; 
    }
    template<typename T>
    Object operator()(const T&) { 
        logger_->log("Invalid unary minus on type: ", typeid(T).name());
        throw std::runtime_error("Invalid operation unary minus");
    }
    static constexpr TokenType token = TokenType::minus;
};
