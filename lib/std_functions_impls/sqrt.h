// add: Sqrt
#pragma once

#include "std_func_include.h"
#include "base_std.h"
#include "object.h"
#include <stdexcept>

struct Sqrt : BaseStd {
    Sqrt() : BaseStd(std::cout) {}
    Sqrt(std::ostream& out, std::vector<Object>&& args);
    ~Sqrt() = default;

    Object Execute();

    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Sqrt does not support this type");
    }
    static constexpr std::string_view name = "sqrt";
    Object operator()(double& arg);
};

