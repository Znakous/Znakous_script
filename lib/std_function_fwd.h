#pragma once
#include <string_view>

// Forward declarations only
struct Print;
struct ParseNum;
struct StdFunc;

bool IsStdFunc(std::string_view s);

constexpr std::string_view std_funcs[] = {
    "print",
    "parsenum"
};