#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: AddVisitor


struct AddVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    AddVisitor() : BinaryOperVisitor(make_ptr<logging::Logger>()) {}
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Adding types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Adding doubles: ", a, " + ", b);
        return a + b; 
    }
    Object operator()(const std::string& a, const std::string& b) { 
        logger_->log("Adding strings: ", a, " + ", b);
        return std::string(a) + b;  
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid addition between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation add " + std::string(typeid(T).name()) + " " + std::string(typeid(U).name()));
    }
    static constexpr TokenType token = TokenType::plus;
};