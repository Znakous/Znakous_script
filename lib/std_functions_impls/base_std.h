#pragma once

#include <vector>
#include <iostream>
#include <optional>
#include <functional>
#include <object.h>
#include "std_func_include.h"

using OptionalRef = std::optional<std::reference_wrapper<std::ostream>>;

struct BaseStd {
    BaseStd() {}
    BaseStd(std::ostream& out_)
        : out(out_)
    {}
    BaseStd(std::vector<Object>&& args_)
     : out(std::nullopt), args(std::move(args_)) 
    {}
    BaseStd(std::ostream& out_, std::vector<Object>&& args_)
        : out(out_), args(std::move(args_))
    {}
    virtual ~BaseStd() = default;
    std::vector<Object> args;
    OptionalRef out;
};