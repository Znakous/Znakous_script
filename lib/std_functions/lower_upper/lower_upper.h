// add: Lower
// add: Upper

#pragma once
#include <string>
#include <cctype>
#include <algorithm>
#include <std_functions/std_func_include.h>
#include <std_functions/base_std.h>
#include "object.h"


enum class StringTransform {
    lower,
    upper
};

template<StringTransform c>
struct LowerUpper : BaseStd {
    LowerUpper() : BaseStd(std::cout) {}
    LowerUpper(std::ostream& out, std::vector<Object>&& args)
     : BaseStd(out, std::move(args)) 
    {}
    ~LowerUpper() = default;


    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("LowerUpper: argument is not a string");
    }

    Object operator()(std::string& a) {
        std::string str = a;
        if constexpr (c == StringTransform::lower) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) { return std::tolower(ch); });
        } else {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) { return std::toupper(ch); });
        }
        return str;
    }
    Object Execute() {
        return std::visit(*this, args[0]);
    }
    static constexpr std::string_view name;
};

template<>
constexpr std::string_view LowerUpper<StringTransform::lower>::name = "lower";

template<>
constexpr std::string_view LowerUpper<StringTransform::upper>::name = "upper";

using Lower = LowerUpper<StringTransform::lower>;
using Upper = LowerUpper<StringTransform::upper>;