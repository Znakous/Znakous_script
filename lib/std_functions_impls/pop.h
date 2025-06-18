// add: Pop

#pragma once
#include "base_std.h"
#include "object.h"

struct Pop : BaseStd {
    Pop() = default;
    ~Pop() = default;
    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Pop: argument is not an array");
    }
    Object operator()(CArray& a);
    Object Execute();
    static constexpr std::string_view name = "pop";
};