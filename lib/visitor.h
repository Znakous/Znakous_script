#pragma once 

#include <string>
#include <variant>
#include <stdexcept>

#include "usings.h"
#include "object.h"


// using Number = double;
// using Value = std::variant<double, std::string, bool, std::monostate>;


struct BinaryOperVisitor {
    virtual ~BinaryOperVisitor() = default;
    virtual Object operator()(Object&, Object&) = 0;
};
struct UnaryOperVisitor {
    virtual ~UnaryOperVisitor() = default;
    virtual Object operator()(Object&) = 0;
};


struct UnaryMinusVisitor : UnaryOperVisitor {
    virtual Object operator()(Object& a) {
        return std::visit(*this, a);
    }
    Object operator()(double a) { 
        return -a; 
    }
    Object operator()(std::string a) { 
        std::reverse(a.begin(), a.end());
        return a; 
    }
    template<typename T>
    Object operator()(T a) { 
        throw std::runtime_error("Invalid operation");
    }
};

struct AddVisitor : BinaryOperVisitor {
    virtual Object operator()(Object& a, Object& b) override {
        return std::visit(*this, a, b);
    }
    Object operator()(double a, double b) {
        std::cout << "add called " << a+b << "\n"; 
        return a+b; 
    }
    Object operator()(std::string a, std::string b) { 
        return a + b;  
    }
    // Object operator()(std::string a, double b) { 
    //     auto ret = make_ptr<Object>();
    //     std::string ans = "";
    //     for (int i = 0; i < b; i++) ans += a;
    //     *ret = ans;
    //     return ret;
    // }
    
    template<typename T, typename U>
    Object operator()(T, U) { 
        throw std::runtime_error("Invalid operation");
    }
};

struct SubVisitor : BinaryOperVisitor {
    virtual Object operator()(Object& a, Object& b) override {
        return std::visit(*this, a, b);
    }
    Object operator()(double a, double b) {
        return a-b; 
    }
    
    template<typename T, typename U>
    Object operator()(T, U) { 
        throw std::runtime_error("Invalid operation");
    }
};

struct MultVisitor : BinaryOperVisitor {
    virtual Object operator()(Object& a, Object& b) override {
        return std::visit(*this, a, b);
    }
    Object operator()(double a, double b) { 
        return a * b; 
    }
    // Object operator()(std::string a, std::string b) { 
    //     auto ret = make_ptr<Object>();
    //     *ret = a + b;
    //     return ret;  
    // }
    Object operator()(std::string a, double b) { 
        std::string ans = "";
        for (int i = 0; i < b; i++) ans += a;
        return ans;
    }
    
    template<typename T, typename U>
    Object operator()(T, U) { 
        throw std::runtime_error("Invalid operation");
    }
};


// std::unordered_map<TokenType, ()
inline Object operator+(Object& a, Object& b) {
    return std::visit(AddVisitor{}, a, b);
}

inline Object operator-(Object& a, Object& b) {
    return std::visit(SubVisitor{}, a, b);
}
inline Object operator*(Object& a, Object& b) {
    return std::visit(MultVisitor{}, a, b);
}