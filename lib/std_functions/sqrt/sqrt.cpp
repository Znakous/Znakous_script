#include "sqrt.h"

Object Sqrt::operator()(double& arg) {
    return std::sqrt(arg);
}

Object Sqrt::Execute() {
    return std::visit(*this, args[0]);
}

