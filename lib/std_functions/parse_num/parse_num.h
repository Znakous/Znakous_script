// add: ParseNum

#pragma once

#include <vector>
#include <iostream>
#include <functional>

#include "object.h"
#include <std_functions/base_std.h>


struct ParseNum : BaseStd {
    ParseNum() : BaseStd(std::cout) {}
    ParseNum(std::ostream&, std::vector<Object>&&);
    ~ParseNum() = default;
    template<typename O>
    Object operator()(O&) {
        throw std::runtime_error("ParseNum does not support this type");
        return CNull{};
    }
    Object operator()(std::string& o);
    Object Execute();
    static constexpr std::string_view name = "parse_num";
};