#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: MultVisitor

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
    Object operator()(const T&, const U&) { 
        logger_->log("Invalid multiplication between types: ", typeid(T).name(), " and ", typeid(U).name());
        throw std::runtime_error("Invalid operation mult");
    }
    static constexpr TokenType token = TokenType::multiply;
};
