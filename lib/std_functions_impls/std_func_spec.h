// Your code was formated to follow the "Google C++ Style Guide" rules of naming functions and constants
#pragma once

#include "trie.h"
#include "std_func_include.h"
#include "../build/std_func_all.h"

using StdFuncVar = std::variant<functions>;

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


StdFuncPtr GetStdFunc(std::string_view s, std::ostream& out, std::vector<Object>&& args);
    
struct Executor {
    template<typename T>
    auto operator()(T& f) {
        return f.Execute();
    }
};

struct OutSetter {
    OutSetter(std::ostream& out) : out(out) {}
    template<typename T>
    void operator()(T& f) {
        f.out = out;
    }
    std::ostream& out;
};

struct ArgsSetter {
    ArgsSetter(std::vector<Object>&& args) : args(std::move(args)) {}
    template<typename T>
    void operator()(T& f) {
        f.args = std::move(args);
    }
    std::vector<Object> args;
};


struct StdFuncGetter {
    StdFuncGetter();
    StdFuncPtr GetStdFunc(std::string_view s, std::ostream& out, std::vector<Object>&& args);
private:
    Trie<StdFunc> functions_;
};