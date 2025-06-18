// add: Read

#pragma once

#include <iostream>

#include "object.h"
#include <std_functions/base_std.h>

struct Read : BaseStd {
    Read() : BaseStd(std::cout) {}
    Object Execute() {
        std::string s;
        std::cin >> s;
        return s;
    }
    static constexpr std::string_view name = "read";
};