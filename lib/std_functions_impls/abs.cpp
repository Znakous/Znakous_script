#include "abs.h"

Abs::Abs(std::ostream& out, std::vector<Object>&& args_)
    : out(out), args(std::move(args_))
{}

Object Abs::operator()(double a) {
    return std::abs(a);
}

Object Abs::Execute() { return std::visit(*this, args[0]); }
