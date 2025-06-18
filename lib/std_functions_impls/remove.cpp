#include "remove.h"

Object Remove::Execute() {
    return std::visit(*this, args[0], args[1]);
}

Object Remove::operator()(CArray& a, double& b) {
    a.arr.erase(a.arr.begin() + b); // чупапимуняня
    return CNull{};
}