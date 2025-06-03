#pragma once

#include <string>
#include <variant>



#include <variant>
#include <memory>
#include <string>

#include "ast/ast.h"

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
