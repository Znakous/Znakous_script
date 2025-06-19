#pragma once

#include <operators\base_operator.h>
#include <token.h>

// add: BitOperVisitor<BitOper::and_>
// add: BitOperVisitor<BitOper::or_>
// add: BitOperVisitor<BitOper::xor_>



template<BitOper oper>
struct BitOperVisitor : BinaryOperVisitor {
    using BinaryOperVisitor::BinaryOperVisitor;
    virtual Object operator()(Object& a, Object& b) override {
        logger_->log("Computing bit operation between types: ", typeid(a).name(), " and ", typeid(b).name());
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
    static constexpr TokenType token;
};

template<>
constexpr Token BitOperVisitor<BitOper::and_>::token = TokenType::and_;

template<>
constexpr Token BitOperVisitor<BitOper::or_>::token = TokenType::or_;

template<>
constexpr Token BitOperVisitor<BitOper::xor_>::token = TokenType::xor_;