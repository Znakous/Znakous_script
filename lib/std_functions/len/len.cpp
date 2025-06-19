#include "len.h"

Object Len::Execute() {
    return std::visit(*this, args[0]);
}

Object Len::operator()(std::string& a) {
    return static_cast<double>(a.size());
}

Object Len::operator()(CArray& a) {
    return static_cast<double>(a.arr.size());
}
