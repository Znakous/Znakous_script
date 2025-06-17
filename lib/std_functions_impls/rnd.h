#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <random>
#include <object.h>
#include "std_func_include.h"

struct Rnd {
    Rnd() : out(std::cout), rng(std::random_device{}()) {}
    Rnd(std::ostream& out, std::vector<Object>&& args);
    ~Rnd() = default;

    template<typename T, typename O>
    Object operator()(T& a, O& b) {
        throw std::runtime_error("Rnd only supports numeric ranges");
        return CNull{};
    }

    Object operator()(double& a, double& b);
    Object Execute();
    
    std::vector<Object> args;
    std::reference_wrapper<std::ostream> out;
private:
    std::mt19937 rng; 
};