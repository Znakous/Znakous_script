#pragma once

#include "object.h"
#include "visitor.h"

struct Printer {
    template<typename T>
    void operator()(T a) {
        std::cout << "print called\n";
        out << a;
    }
    void operator()(CArray a) {
        out << "[";
        for (size_t i = 0; i < a.arr.size(); ++i) {
            std::visit(Printer(out), a.arr[i]);
        }
        out << "]";
    }
    std::ostream& out;
};

struct Print {
    Object operator()(Object o) {
        std::visit(Printer(out), o);
        return o;
    } 
    std::ostream& out;
};

struct ParseNum {
    Object operator()(Object o) {
        return std::stod(std::get<std::string>(o));
    }
};



struct TruthChecker {
    bool operator()(double a);
    bool operator()(const std::string& s);
    bool operator()(CNull);
    bool operator()(BuiltinFuncPtr);
    bool operator()(CArray a);
};


bool IsStdFunc(std::string_view s);