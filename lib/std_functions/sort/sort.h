// add: Sort

#pragma once
#include <std_functions/base_std.h>
#include "object.h"
#include <operators/operators.h>

struct Sort : BaseStd {
    ~Sort() = default;
    template<typename T>
    Object operator()(const T&) {
        throw std::runtime_error("Sort: argument is not an array");
    }
    Object operator()(CArray& a);
    Object Execute();
    static constexpr std::string_view name = "sort";
};