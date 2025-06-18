// add: Round<RoundingMode::floor>
// add: Round<RoundingMode::ceil>
// add: Round<RoundingMode::common>

#pragma once

#include "std_func_include.h"
#include "base_std.h"
#include "object.h"
#include <stdexcept>

template<RoundingMode mode>
struct Round : BaseStd {
    Round() : BaseStd(std::cout) {}
    Round(std::ostream& out, std::vector<Object>&& args);
    ~Round() = default;

    Object Execute() {
        return std::visit(*this, args[0]);
    }

    template<typename T>
    Object operator()(T& a) {
        throw std::runtime_error("Round does not support this type");
    }

    Object operator()(double& arg) {
        if constexpr (mode == RoundingMode::common) {
            return Object(std::round(arg));
        } else if constexpr (mode == RoundingMode::floor) {
            return Object(std::floor(arg));
        } else if constexpr (mode == RoundingMode::ceil) {
            return Object(std::ceil(arg));
        } else {
            throw std::runtime_error("Invalid rounding mode");
        }
    }
    static constexpr std::string_view name;
};

template<>
constexpr std::string_view Round<RoundingMode::common>::name = "round";
template<>
constexpr std::string_view Round<RoundingMode::floor>::name = "floor";
template<>
constexpr std::string_view Round<RoundingMode::ceil>::name = "ceil";



