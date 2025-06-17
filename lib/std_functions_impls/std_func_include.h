#pragma once

#define FUNC_MAPPING(name, struct_name) functions_.insert(#name, StdFunc{struct_name{}});
#include <string_view>
#include "usings.h"


constexpr std::string_view std_funcs[] = {
    "print",
    "parsenum", 
    "abs",
    "rnd",
    "len"
};

// for variant
#define functions Print, ParseNum, Abs, Rnd, Len

// forward declarations
struct Print;
struct ParseNum;
struct Abs;
struct Rnd;
struct Len;

// name to functor mapping
#define LIST_OF_FUNCS_TO_INSERT(name) \
    FUNC_MAPPING(print, Print) \
    FUNC_MAPPING(parse_num, ParseNum) \
    FUNC_MAPPING(abs, Abs) \
    FUNC_MAPPING(rnd, Rnd) \
    FUNC_MAPPING(len, Len) \
;

struct StdFunc;
using StdFuncPtr = ptr<StdFunc>;


