// add: Remove

#pragma once
#include <std_functions/base_std.h>
#include <object.h>

struct Remove : BaseStd {
    ~Remove() = default;
    template<typename T, typename U>
    Object operator()(const T&, const U&) {
        throw std::runtime_error("Remove: argument is not an array");
    }
    Object operator()(CArray& a, const double& b);
    Object Execute();
    static constexpr std::string_view name = "remove";
};