#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <functional>

#include <object.h>
#include "std_func_include.h"


struct Abs {
    Abs() : out(std::cout) {}
    Abs(std::ostream& out, std::vector<Object>&& args);
    ~Abs() = default;
    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Abs does not support this type");
        return CNull{};
    }

    Object operator()(double a);

    Object Execute();
    
    std::vector<Object> args;
    std::reference_wrapper<std::ostream> out;
};