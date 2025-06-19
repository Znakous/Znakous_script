#pragma once

#include <operators\base_operator.h>
#include <token.h>
#include <iostream>

// add: BitOperVisitor<BitOper::and_>
// add: BitOperVisitor<BitOper::or_>
// add: BitOperVisitor<BitOper::xor_>



template<BitOper oper>
struct BitOperVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        return std::visit(*this, a, b);
    }
    Object operator()(const double& a_double, const double& b_double) {
        int64_t a = static_cast<int64_t>(a_double);
        int64_t b = static_cast<int64_t>(b_double);
        if constexpr (oper == BitOper::and_) {
            return static_cast<double>(a & b);
        } else if constexpr (oper == BitOper::or_) {
            return static_cast<double>(a | b);
        } else if constexpr (oper == BitOper::xor_) {
            return static_cast<double>(a ^ b);
        }
        throw std::runtime_error("Invalid bit operation");
    }
    template<typename T, typename U>
    Object operator()(const T&, const U&) {
        throw std::runtime_error("Invalid operation bit");
    }
    static constexpr TokenType token = TokenType::invalid;
};

template<>
constexpr TokenType BitOperVisitor<BitOper::and_>::token = TokenType::bit_and;

template<>
constexpr TokenType BitOperVisitor<BitOper::or_>::token = TokenType::bit_or;

template<>
constexpr TokenType BitOperVisitor<BitOper::xor_>::token = TokenType::bit_xor;