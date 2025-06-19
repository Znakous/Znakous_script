#include "insert.h"

Object Insert::Execute() {
    return std::visit(*this, args[0], args[1], args[2]);
}

Object Insert::operator()(CArray& a, double& b, double& c) {
    a.arr.insert(a.arr.begin() + b, c);
    return CNull{};
}