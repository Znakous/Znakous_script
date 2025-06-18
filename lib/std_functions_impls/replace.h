// add: Replace

#pragma once
#include "base_std.h"
#include "object.h"

struct Replace : BaseStd {
    Replace() : BaseStd() 
    {}
    ~Replace() = default;
    template<typename T, typename Old, typename New>
    Object operator()(T& a, Old& old, New& new_) {
        throw std::runtime_error("Replace: argument is not a string or array");
    }
    Object operator()(std::string& a, std::string& old, std::string& new_);
    Object Execute();
    static constexpr std::string_view name = "replace";
};