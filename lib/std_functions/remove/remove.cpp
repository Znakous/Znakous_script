#include "remove.h"

Object Remove::Execute() {
    if (args.size() != 2) {
        throw std::runtime_error(
            "Remove: invalid number of arguments, expected 2, got " + std::to_string(args.size())
        );
    }
    if (!std::holds_alternative<CArray>(args[0])) {
        throw std::runtime_error("Remove: first argument must be an array");
    }
    if (!std::holds_alternative<double>(args[1])) {
        throw std::runtime_error("Remove: second argument must be a number");
    }
    return std::visit(*this, args[0], args[1]);
}

Object Remove::operator()(CArray& a, const double& b) {
    a.arr.erase(a.arr.begin() + b);
    return CNull{};
}