// add: Replace

#pragma once
#include <std_functions/base_std.h>
#include <object.h>

struct Replace : BaseStd {
    ~Replace() = default;
    template<typename T, typename Old, typename New>
    Object operator()(const T&, const Old&, const New&) {
        throw std::runtime_error("Replace: argument is not a string or array");
    }
    Object operator()(const std::string& a, const std::string& old, const std::string& new_);
    Object Execute();
    static constexpr std::string_view name = "replace";
};