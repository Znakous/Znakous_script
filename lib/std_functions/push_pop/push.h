// add: Push

#pragma once
#include <std_functions/base_std.h>
#include "object.h"

struct Push : BaseStd {
    ~Push() = default;
    template<typename T, typename U>
    Object operator()(const T&, const U&) {
        throw std::runtime_error("Push: argument is not an array");
    }
    template<typename T>
    Object operator()(CArray& a, T&& b) {
        a.arr.push_back(b);
        return a;
    }

    Object Execute();
    static constexpr std::string_view name = "push";
};

