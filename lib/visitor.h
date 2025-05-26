#pragma once 

#include <string>
#include <variant>

#include "usings.h"
#include "object.h"


using Number = double;
using Value = std::variant<Number, String, bool, std::monostate>;


struct AddVisitor {
    ptr<Object> operator()(Number a, Number b) { 
        auto ret = make_ptr<Integer>();
        ret->value = a + b;
        return ret; 
    }
    ptr<Object> operator()(std::string a, std::string b) { 
        auto ret = make_ptr<String>();
        ret->value = a + b;
        return ret; 
    }
    ptr<Object> operator()(std::string a, Number b) { 
        auto ret = make_ptr<String>();
        for (int i = 0; i < b; i++) ret->value += a;
        return ret;
    }
    // ... другие комбинации
    
    template<typename T, typename U>
    ptr<Object> operator()(T, U) { 
        throw RuntimeError("Invalid operation");
    }
};


inline ptr<Object> operator+(const ptr<Object>& a, const ptr<Object>& b) {
    return std::visit(AddVisitor{}, a->getValue(), b->getValue());
}