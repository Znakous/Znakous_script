// add: Len

#pragma once

#include <std_functions/base_std.h>
#include "object.h"

struct Len : BaseStd {
    Len() : BaseStd(std::cout) {}
    Len(std::ostream& out, std::vector<Object>&& args);
    ~Len() = default;
    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Len does not support this type");
        return CNull{};
    }

    Object operator()(std::string& a);
    Object operator()(CArray& a);
    Object Execute();
    static constexpr std::string_view name = "len";
};

