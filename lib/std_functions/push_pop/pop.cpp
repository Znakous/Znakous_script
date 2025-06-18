#include "pop.h"

Object Pop::Execute() {
    return std::visit(*this, args[0]);
}

Object Pop::operator()(CArray& a) {
    if (a.arr.empty()) {
        throw std::runtime_error("Pop: array is empty");
    }
    Object ans = a.arr.back();
    a.arr.pop_back();
    return a;
}