#pragma once

#include <string>
#include <variant>
#include <memory>
#include <vector>
#include <ostream>
#include "ast/expressions/expression_fwd.h"
#include "std_function_fwd.h"

struct CNull {};

inline std::ostream& operator<<(std::ostream& out, CNull) {
    out << "Null";
    return out;
}

using BuiltinFuncPtr = ptr<FunctionalExpression>;
using BuiltinArray = ptr<ArrayExpression>;
using StdFuncPtr = std::shared_ptr<StdFunc>;

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
