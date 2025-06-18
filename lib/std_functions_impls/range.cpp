#include "range.h"

Object Range::Execute() {
    if (args.size() == 1) {
        return std::visit(*this, Object(0.0), args[0], Object(1.0));
    } else if (args.size() == 2) {
        return std::visit(*this, args[0], args[1], Object(1.0));
    } else if (args.size() == 3) {
        return std::visit(*this, args[0], args[1], args[2]);
    }
    throw std::runtime_error("Range: wrong number of arguments");
}

Object Range::operator()(double a, double b, double c) {
    CArray ans;
    for (double i = a; i < b; i += c) {
        ans.arr.push_back(i);
    }
    return ans;
}