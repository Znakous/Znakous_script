#pragma once

#include <vector>
#include <iostream>
#include <functional>

#include <object.h>
#include "std_func_include.h"


struct ParseNum {
    ParseNum() : out(std::cout) {}
    ParseNum(std::ostream&, std::vector<Object>&&);
    ~ParseNum() = default;
    template<typename O>
    Object operator()(O&) {
        throw std::runtime_error("ParseNum does not support this type");
        return CNull{};
    }
    Object operator()(std::string& o);
    Object Execute();
    
    std::vector<Object> args;
    std::reference_wrapper<std::ostream> out;
};