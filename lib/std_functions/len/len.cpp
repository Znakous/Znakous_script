#include "len.h"

Object Len::Execute() {
    return std::visit(*this, args[0]);
}

Object Len::operator()(const std::string& a) {
    return static_cast<double>(a.size());
}

Object Len::operator()(const CArray& a) {
    return static_cast<double>(a.arr.size());
}
