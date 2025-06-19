#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: PowerVisitor

struct PowerVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Computing power operation between types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Computing power: ", a, " ^ ", b);
        return std::pow(a, b);
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid power operation between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation power");
    }
    static constexpr TokenType token = TokenType::power;
};
