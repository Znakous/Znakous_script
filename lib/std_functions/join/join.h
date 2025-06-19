// add: Join
#pragma once
#include <string>
#include <vector>
#include <std_functions/std_func_include.h>
#include <std_functions/base_std.h>
#include "object.h"


struct Join : BaseStd {
    ~Join() = default;

    template<typename T, typename O>
    Object operator()(const T&, const O&) {
        throw std::runtime_error("Join: argument is not a string");
    }

    Object operator()(CArray& a, std::string& b);
    Object Execute();
    static constexpr std::string_view name="join";
};