// add: Print

#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "object.h"
#include <std_functions/base_std.h>

struct Print : BaseStd {
    ~Print() = default;
    template<typename T>
    void operator()(const T& a) {
        out->get() << a;
    };
    void operator()(CArray& a);
    void operator()(StdFuncPtr& a);
    Object Execute();
    
    static constexpr std::string_view name = "print";
};