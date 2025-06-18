// add: Insert

#pragma once
#include "base_std.h"
#include "object.h"

struct Insert : BaseStd {
    Insert() = default;
    ~Insert() = default;
    template<typename T, typename U, typename V>
    Object operator()(T& a, U& b, V& c) {
        throw std::runtime_error("Insert: argument is not an array");
    }
    Object operator()(CArray& a, double& b, double& c);
    Object Execute();
    static constexpr std::string_view name = "insert";
};