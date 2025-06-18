// add: Push

#pragma once
#include "base_std.h"
#include "object.h"

struct Push : BaseStd {
    Push() = default;
    ~Push() = default;
    template<typename T, typename U>
    Object operator()(T& a, U& b) {
        throw std::runtime_error("Push: argument is not an array");
    }
    template<typename T>
    Object operator()(CArray& a, T& b) {
        a.arr.push_back(b);
        return a;
    }

    Object Execute();
    static constexpr std::string_view name = "push";
};

