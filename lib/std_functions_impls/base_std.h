#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <object.h>
#include "std_func_include.h"

struct BaseStd {
    BaseStd(std::ostream& out)
        : out(out)
    {}
    BaseStd(std::ostream& out, std::vector<Object>&& args)
        : out(out), args(std::move(args))
    {}
    virtual ~BaseStd() = default;
    std::vector<Object> args;
    std::reference_wrapper<std::ostream> out;
};