#pragma once

#include <string>
#include <variant>
#include <memory>
#include <vector>
#include <ostream>
#include "ast/expressions/expression_fwd.h"
#include "std_functions_impls/std_func_include.h"

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

struct Object : std::variant<
    double,
    std::string,
    CNull,
    BuiltinFuncPtr,
    CArray,
    StdFuncPtr
> {
    using std::variant<
        double,
        std::string,
        CNull,
        BuiltinFuncPtr,
        CArray,
        StdFuncPtr
    >::variant;
};

