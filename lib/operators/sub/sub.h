#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: SubVisitor

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
    Object operator()(const T&, const U&) { 
        throw std::runtime_error("Invalid operation sub");
    }
    static constexpr TokenType token = TokenType::minus;

};