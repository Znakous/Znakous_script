// add: Print

#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "object.h"
#include "base_std.h"

struct Print : BaseStd {
    Print() : BaseStd(std::cout) {}
    Print(std::ostream& out, std::vector<Object>&& args);
    
    ~Print() = default;
    template<typename T>
    void operator()(T& a) {
        out->get() << a;
    };
    void operator()(CArray& a);
    void operator()(StdFuncPtr& a);
    Object Execute();
    
    static constexpr std::string_view name = "print";
};