#pragma once

#include <iostream>
#include <variant>
#include <memory>
#include "std_function_fwd.h"
#include "object.h"


struct Print {
    Print(std::ostream& out, std::vector<Object>&& args)
     : out(out), args_(std::move(args))
    {}
    template<typename T>
    void operator()(T& a) {
        out << a;
    }
    void operator()(CArray& a) {
        out << "[";
        for (size_t i = 0; i < a.arr.size(); ++i) {
            std::visit(*this, a.arr[i]);
            if (i != a.arr.size() - 1) {
                out << ", ";
            }
        }
        out << "]";
    }
    void operator()(StdFuncPtr& a) {
        out << "<std_function>";
    }
    Object Execute() { 
        for (auto& arg : args_) {
            std::visit(*this, arg);
        }
        return args_[0];
    }
    std::vector<Object> args_;
    std::ostream& out;
};

struct ParseNum {
    ParseNum(std::ostream& out, std::vector<Object>&& args)
     : out(out), args_(std::move(args))
    {}
    template<typename O>
    Object operator()(O&) {
        throw std::runtime_error("ParseNum does not support this type");
        return CNull{};
    }
    Object operator()(std::string& o) {
        return std::stod(o);
    }
    Object Execute() { return std::visit(*this, args_[0]); }
    std::vector<Object> args_;
    std::ostream& out;
};

using StdFuncVar = std::variant<Print, ParseNum>;

struct StdFunc : StdFuncVar {
    using StdFuncVar::StdFuncVar;
};

struct TruthChecker {
    bool operator()(double a);
    bool operator()(const std::string& s);
    bool operator()(CNull);
    bool operator()(BuiltinFuncPtr);
    bool operator()(CArray a);
    bool operator()(const StdFuncPtr& f);
};

bool IsStdFunc(std::string_view s);

StdFuncPtr GetStdFunc(std::string_view s, std::ostream& out, std::vector<Object>&& args);
    
struct Executor {
    template<typename T>
    auto operator()(T& f) {
        return f.Execute();
    }
};

