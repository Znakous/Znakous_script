#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <object.h>
#include "std_func_include.h"

struct Print {
    Print() : out(std::cout) {}
    Print(std::ostream& out, std::vector<Object>&& args);
    
    ~Print() = default;
    template<typename T>
    void operator()(T& a) {
        out.get() << a;
    };
    void operator()(CArray& a);
    void operator()(StdFuncPtr& a);
    Object Execute();
    
    std::vector<Object> args;
    std::reference_wrapper<std::ostream> out;
};