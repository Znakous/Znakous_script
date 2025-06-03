#pragma once

#include <string>
#include <variant>
#include <memory>
#include <ostream>
#include "ast/expressions/expression_fwd.h"

// #include <ast/statements/statement.h>

struct CNull{};

inline std::ostream& operator<<(std::ostream& out, CNull) {
    out << "Null";
    return out;
}

using BuiltinFuncPtr = ptr<FunctionalExpression>;

using Object = std::variant<
    double,
    std::string,
    CNull,  // для Null
    BuiltinFuncPtr
    // ,
    // std::vector<std::variant<double,
    // std::string,
    // std::monostate>>
>;
