#include "len.h"

Len::Len(std::ostream& out, std::vector<Object>&& args) : BaseStd(out, std::move(args)) {}

Object Len::Execute() {
    return std::visit(*this, args[0]);
}

Object Len::operator()(std::string& a) {
    return static_cast<double>(a.size());
}

Object Len::operator()(CArray& a) {
    return static_cast<double>(a.arr.size());
}
