#pragma once

#include <string>
#include <variant>



#include <variant>
#include <memory>
#include <string>

#include "ast/ast.h"


using BuiltinFuncPtr = FunctionalExpression* ;

using Object = std::variant<
    double,         // для Integer
    std::string,    // для String
    std::monostate,  // для Null
    BuiltinFuncPtr
>;
