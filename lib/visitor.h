#pragma once 

#include <string>
#include <variant>
#include <stdexcept>

#include "usings.h"
#include "object.h"
#include "logger.h"


// using Number = double;
// using Value = std::variant<double, std::string, bool, std::monostate>;


struct BinaryOperVisitor {
    BinaryOperVisitor(std::shared_ptr<logging::Logger> log)
     : logger_(log) 
    {}
    virtual ~BinaryOperVisitor() = default;
    virtual Object operator()(Object&, Object&) = 0;
protected:
    std::shared_ptr<logging::Logger> logger_;
};


struct UnaryOperVisitor {
    UnaryOperVisitor(std::shared_ptr<logging::Logger> log)
     : logger_(log) 
    {}
    virtual ~UnaryOperVisitor() = default;
    virtual Object operator()(Object&) = 0;
protected:
    std::shared_ptr<logging::Logger> logger_;
};


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
    Object operator()(const T& t) { 
        logger_->log("Invalid unary minus on type: ", typeid(T).name());
        throw std::runtime_error("Invalid operation unary minus");
    }
};

struct AddVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
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
    Object operator()(const double& a, const std::string& b) {
        logger_->log("Adding double to string: ", a, " + ", b);
        return std::to_string(a) + b;
    }
    Object operator()(const std::string& a, const double& b) {
        logger_->log("Adding string to double: ", a, " + ", b);
        return a + std::to_string(b);
    }
    template<typename T, typename U>
    Object operator()(const T& t, const U& u) { 
        logger_->log("Invalid addition between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation add " + std::string(typeid(T).name()) + " " + std::string(typeid(U).name()));
    }
};

struct SubVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        return std::visit(*this, a, b);
    }
    Object operator()(double a, double b) {
        return a-b; 
    }

    Object operator()(std::string& a, std::string& b) { 
        if (a.ends_with(b)) {
            return std::string(a.substr(0, a.size() - b.size()));
        }
        return std::string(a);
    }
    
    template<typename T, typename U>
    Object operator()(T&&, U&&) { 
        throw std::runtime_error("Invalid operation sub");
    }
};

struct MultVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Multiplying types: ", typeid(a).name(), " and ", typeid(b).name());
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a, const double& b) { 
        logger_->log("Multiplying doubles: ", a, " * ", b);
        return a * b; 
    }
    Object operator()(const std::string& a, const double& b) { 
        logger_->log("Multiplying string by double: ", a, " * ", b);
        std::string ans;
        ans.reserve(a.size() * static_cast<size_t>(b));
        for (int i = 0; i < b; i++) ans += a;
        return ans;
    }
    Object operator()(const double& a, const std::string& b) {
        logger_->log("Multiplying double by string: ", a, " * ", b);
        std::string ans;
        ans.reserve(b.size() * static_cast<size_t>(a));
        for (int i = 0; i < a; i++) ans += b;
        return ans;
    }
    template<typename T, typename U>
    Object operator()(const T& t, const U& u) { 
        logger_->log("Invalid multiplication between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation mult");
    }
};

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
    Object operator()(const T& t, const U& u) { 
        logger_->log("Invalid division between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation divide");
    }
};


// inline Object operator+(Object& a, Object& b) {
//     return std::visit(AddVisitor{}, a, b);
// }

// inline Object operator-(Object& a, Object& b) {
//     return std::visit(SubVisitor{}, a, b);
// }
// inline Object operator*(Object& a, Object& b) {
//     return std::visit(MultVisitor{}, a, b);
// }