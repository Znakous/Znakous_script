// add: Range

#pragma once
#include <std_functions/base_std.h>
#include "object.h" 

struct Range : BaseStd {
    Range() : BaseStd() {}
    Range(std::vector<Object>&& args) : BaseStd(std::move(args)) {}
    Range(std::ostream& out, std::vector<Object>&& args) : BaseStd(out, std::move(args)) {}
    ~Range() = default;
    template<typename T, typename U, typename V>
    Object operator()(const T& a, const U& b, const V& c) {
        throw std::runtime_error("Range: argument is not a number");
    }
    Object operator()(double a, double b, double c);
    Object Execute();
    static constexpr std::string_view name = "range";
};
