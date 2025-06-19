#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: ModVisitor

struct ModVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Computing modulo operation between types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Computing modulo: ", a, " % ", b);
        if (b == 0) {
            throw std::runtime_error("Division by zero in modulo operation");
        }
        return std::fmod(a, b);
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid modulo operation between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation modulo");
    }
    static constexpr TokenType token = TokenType::mod;
};