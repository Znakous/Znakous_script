#pragma once

#define FUNC_MAPPING(name, struct_name) functions_.insert(#name, StdFunc{struct_name{}});
#include <string_view>
#include "usings.h"

// for variant
#define functions Print, ParseNum, Abs

// forward declarations
struct Print;
struct ParseNum;
struct Abs;
// struct Rnd;

// name to functor mapping
#define LIST_OF_FUNCS_TO_INSERT(name) \
    FUNC_MAPPING(print, Print) \
    FUNC_MAPPING(parse_num, ParseNum) \
    FUNC_MAPPING(abs, Abs) \ 
;

struct StdFunc;
using StdFuncPtr = ptr<StdFunc>;

constexpr std::string_view std_funcs[] = {
    "print",
    "parsenum", 
    "abs"
};