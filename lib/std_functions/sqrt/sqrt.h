// add: Sqrt
#pragma once

#include <std_functions/std_func_include.h>
#include <std_functions/base_std.h>
#include "object.h"
#include <stdexcept>

struct Sqrt : BaseStd {
    ~Sqrt() = default;

    Object Execute();

    template<typename T>
    Object operator()(const T&) {
        throw std::runtime_error("Sqrt does not support this type");
    }
    static constexpr std::string_view name = "sqrt";
    Object operator()(double& arg);
};

