// add: Abs
#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <functional>

#include <object.h>
#include <std_functions/base_std.h>


struct Abs : BaseStd {
    ~Abs() = default;
    template<typename T>
    Object operator()(const T&) {
        throw std::runtime_error("Abs does not support this type");
        return CNull{};
    }

    Object operator()(double a);

    Object Execute();
    
    static constexpr std::string_view name = "abs";
};