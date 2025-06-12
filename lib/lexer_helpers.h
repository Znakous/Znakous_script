#pragma once

#include <bitset>
#include <cctype>

constexpr std::bitset<256> make_char_bitset(const char* str);


bool is_forbidden_char(char c) noexcept;

bool is_forbidden_startchar(char c) noexcept;

bool is_whitespace(char c) noexcept;

bool is_numeric(char c) noexcept;