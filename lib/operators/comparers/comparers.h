#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: EqVisitor
// add: NeqVisitor
// add: LeqVisitor
// add: GeqVisitor
// add: LessVisitor
// add: GreaterVisitor


struct EqVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Comparing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " == ", b);
        return a == b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " == ", b);
        return a == b ? True_val : False_val;
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation eq");
    }
    static constexpr TokenType token = TokenType::eq
};

struct NeqVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Comparing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " != ", b);
        return a != b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " != ", b);
        return a != b ? True_val : False_val;
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation neq");
    }
    static constexpr TokenType token = TokenType::neq;
};

struct LessVisitor : BinaryOperVisitor {
    LessVisitor() : BinaryOperVisitor(make_ptr<logging::Logger>()) {}
    LessVisitor(std::shared_ptr<logging::Logger> log)
     : BinaryOperVisitor(log) 
    {}
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Comparing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " < ", b);
        return a < b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " < ", b);
        return a < b ? True_val : False_val;   
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation less");
    }
    static constexpr TokenType token = TokenType::less
};

struct GreaterVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Comparing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " > ", b);
        return a > b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " > ", b);
        return a > b ? True_val : False_val;    
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation greater");
    }
    static constexpr TokenType token = TokenType::greater
};

struct LeqVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Comparing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " <= ", b);
        return a <= b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " <= ", b);
        return a <= b ? True_val : False_val;   
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation leq");
    }
    static constexpr TokenType token = TokenType::leq;
};

struct GeqVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Comparing types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) {
        logger_->log("Comparing doubles: ", a, " >= ", b);
        return a >= b ? True_val : False_val;
    }
    Object operator()(const std::string& a, const std::string& b) {
        logger_->log("Comparing strings: ", a, " >= ", b);
        return a >= b ? True_val : False_val;   
    }   
    template<typename T, typename U>
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid comparison between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation geq");
    }
    static constexpr TokenType token = TokenType::geq;
};    