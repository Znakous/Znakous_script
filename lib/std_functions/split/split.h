// add: Split

#pragma once
#include <string>
#include <vector>
#include <std_functions/std_func_include.h>
#include <std_functions/base_std.h>
#include "object.h"

struct Split : BaseStd {
    ~Split() = default;

    template<typename T, typename O>
    Object operator()(const T&, const O&) {
        throw std::runtime_error("Split: argument is not a string");
    }

    Object operator()(std::string& a, std::string& b);
    Object Execute();
    static constexpr std::string_view name="split";
};

