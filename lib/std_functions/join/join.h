// add: Join
#pragma once
#include <string>
#include <vector>
#include <std_functions/std_func_include.h>
#include <std_functions/base_std.h>
#include "object.h"


struct Join : BaseStd {
    Join() : BaseStd(std::cout) {}
    Join(std::ostream& out, std::vector<Object>&& args) : BaseStd(out, std::move(args)) {}
    ~Join() = default;

    template<typename T, typename O>
    Object operator()(T& a, O& b) {
        throw std::runtime_error("Join: argument is not a string");
    }

    Object operator()(CArray& a, std::string& b);
    Object Execute();
    static constexpr std::string_view name="join";
};