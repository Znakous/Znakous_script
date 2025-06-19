// add: Rnd

#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <random>
#include <object.h>
#include <std_functions/std_func_include.h>
#include <std_functions/base_std.h>

struct Rnd : BaseStd {
    ~Rnd() = default;

    template<typename T, typename O>
    Object operator()(const T&, const O&) {
        throw std::runtime_error("Rnd only supports numeric ranges");
        return CNull{};
    }

    Object operator()(double& a, double& b);
    Object Execute();

    static constexpr std::string_view name = "rnd";
private:
    std::mt19937 rng; 
};