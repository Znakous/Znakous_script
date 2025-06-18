// add: ToString

#pragma once

#include "std_func_include.h"
#include "base_std.h"
#include "object.h"
#include <stdexcept>

struct ToString : BaseStd {
    ToString() : BaseStd(std::cout) {}
    ToString(std::ostream& out, std::vector<Object>&& args);
    ~ToString() = default;

    Object Execute();

    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Sqrt does not support this type");
    }

    Object operator()(double& arg);
    static constexpr std::string_view name = "to_string";
};

