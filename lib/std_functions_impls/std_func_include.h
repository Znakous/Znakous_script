#pragma once

#define FUNC_MAPPING(name, struct_name) functions_.insert(#name, StdFunc{struct_name{}});
#include <string_view>
#include "usings.h"


constexpr std::string_view std_funcs[] = {
    "print",
    "parse_num", 
    "abs",
    "rnd",
    "len", 
    "floor",
    "ceil",
    "round", 
    "sqrt",
    "to_string"
};

// for variant
#define functions Print, ParseNum, Abs, Rnd, Len, \
    Round<RoundingMode::floor>, Round<RoundingMode::ceil>, Round<RoundingMode::common>, \
    Sqrt, ToString

// forward declarations
struct Print;
struct ParseNum;
struct Abs;
struct Rnd;
struct Len;

template<RoundingMode mode> 
struct Round;
struct Sqrt;
struct ToString;

// name to functor mapping
#define LIST_OF_FUNCS_TO_INSERT(name) \
    FUNC_MAPPING(print, Print) \
    FUNC_MAPPING(parse_num, ParseNum) \
    FUNC_MAPPING(abs, Abs) \
    FUNC_MAPPING(rnd, Rnd) \
    FUNC_MAPPING(len, Len) \
    FUNC_MAPPING(floor, Round<RoundingMode::floor>) \
    FUNC_MAPPING(ceil, Round<RoundingMode::ceil>) \
    FUNC_MAPPING(round, Round<RoundingMode::common>) \
    FUNC_MAPPING(sqrt, Sqrt) \
    FUNC_MAPPING(to_string, ToString)
;

struct StdFunc;
using StdFuncPtr = ptr<StdFunc>;


