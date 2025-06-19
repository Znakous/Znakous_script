#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: BinaryAndVisitor


struct BinaryAndVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " && ", b);
        return a && b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " && ", b);
        return (!a.empty()) && (!b.empty()) ? True_val : False_val;   
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation binary and");
    }
    static constexpr TokenType token = TokenType::and_;
};
