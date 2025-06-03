#pragma once

#include "object.h"
#include "visitor.h"

struct Printer {
    template<typename T>
    void operator()(T a) {
        out << a;
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

// using std_func = std::variant<Printer, ParseNum>;



struct TruthChecker {
    bool operator()(double a);
    bool operator()(const std::string& s);
    bool operator()(CNull);
    bool operator()(BuiltinFuncPtr);
};


bool IsStdFunc(std::string_view s);