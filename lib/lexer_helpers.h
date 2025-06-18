#pragma once

#include <bitset>
#include <cctype>
#include <string_view>
#include "std_functions_impls/get_name.h"
#include "std_functions_impls/std_func_spec.h"

const std::vector<std::string_view> std_funcs = []() {
    auto names = GetNames<StdFuncVar, StdFuncVar>().names;
    std::vector<std::string_view> names_view;
    for (auto& name : names) {
        names_view.push_back(name.name);
    }
    return names_view;
}();

constexpr std::bitset<256> make_char_bitset(const char* str);


bool is_forbidden_char(char c) noexcept;

bool is_forbidden_startchar(char c) noexcept;

bool is_whitespace(char c) noexcept;

bool is_numeric(char c) noexcept;