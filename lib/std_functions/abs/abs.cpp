#include "abs.h"


Object Abs::operator()(double a) {
    return std::abs(a);
}

Object Abs::Execute() { return std::visit(*this, args[0]); }
