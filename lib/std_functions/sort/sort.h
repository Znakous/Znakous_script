// add: Sort

#pragma once
#include <std_functions/base_std.h>
#include "object.h"
#include "visitor.h"

struct Sort : BaseStd {
    Sort() = default;
    ~Sort() = default;
    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Sort: argument is not an array");
    }
    Object operator()(CArray& a);
    Object Execute();
    static constexpr std::string_view name = "sort";
};