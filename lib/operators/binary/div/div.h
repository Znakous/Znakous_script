#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: DivideVisitor

struct DivideVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Dividing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(double a, double b) {
        logger_->log("Dividing doubles: ", a, " / ", b);
        return a / b;
    }  
    Object operator()(const std::string& a, const double& b) {
        logger_->log("Dividing string by double: ", a, " / ", b);
        size_t cut = static_cast<size_t>(a.size() / b);
        return std::string(a.substr(0, cut));
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid division between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation divide");
    }
    static constexpr TokenType token = TokenType::divide;
};