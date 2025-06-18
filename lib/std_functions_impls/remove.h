// add: Remove

#pragma once
#include "base_std.h"
#include "object.h"

struct Remove : BaseStd {
    Remove() = default;
    ~Remove() = default;
    template<typename T, typename U>
    Object operator()(T& a, U& b) {
        throw std::runtime_error("Remove: argument is not an array");
    }
    Object operator()(CArray& a, double& b);
    Object Execute();
    static constexpr std::string_view name = "remove";
};