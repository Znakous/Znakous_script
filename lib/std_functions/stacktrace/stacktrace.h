// add: Stacktrace
#pragma once
#include <std_functions/base_std.h>
#include "object.h"

struct Stacktrace : BaseStd {
    Stacktrace() = default;
    ~Stacktrace() = default;
    Object Execute();
    static constexpr std::string_view name = "stacktrace";
};