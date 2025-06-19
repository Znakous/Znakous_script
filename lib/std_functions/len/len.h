// add: Len

#pragma once

#include <std_functions/base_std.h>
#include "object.h"

struct Len : BaseStd {
    ~Len() = default;
    template<typename T>
    Object operator()(const T&) {
        throw std::runtime_error("Len does not support this type");
        return CNull{};
    }

    Object operator()(const std::string& a);
    Object operator()(const CArray& a);   
    Object Execute();
    static constexpr std::string_view name = "len";
};

