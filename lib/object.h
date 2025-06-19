#pragma once

#include <string>
#include <variant>
#include <memory>
#include <vector>
#include <ostream>
#include "ast/expressions/expression_fwd.h"
#include <std_functions/std_func_include.h>

struct CNull {};

inline std::ostream& operator<<(std::ostream& out, CNull) {
    out << "Null";
    return out;
}

using BuiltinFuncPtr = ptr<FunctionalExpression>;
using BuiltinArray = ptr<ArrayExpression>;


struct Object;

struct CArray {
    std::vector<Object> arr;
};

using ObjVar = std::variant<
    double,
    std::string,
    CNull,
    BuiltinFuncPtr,
    CArray,
    StdFuncPtr
>;

struct Object : ObjVar {
    using ObjVar::variant;
};

