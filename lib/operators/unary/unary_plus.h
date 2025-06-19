#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: UnaryPlusVisitor

struct UnaryPlusVisitor : UnaryOperVisitor {
    using UnaryOperVisitor::UnaryOperVisitor;
    virtual Object operator()(Object& a) {
        logger_->log("Unary Plus on type: ", typeid(a).name());
        auto result = std::visit(*this, a);
        logger_->log("Unary Plus result index: ", result.index());
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
    Object operator()(const T& t) { 
        logger_->log("Why did you actually call an unary plus");
        return t;
    }
    static constexpr TokenType token = TokenType::plus;

};
