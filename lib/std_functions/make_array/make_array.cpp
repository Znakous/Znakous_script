#include "make_array.h"
#include <stdexcept>

Object MakeArray::Execute() {
    if (!std::holds_alternative<double>(args[0])) {
        throw std::runtime_error("MakeArray: first argument must be a number");
    }
    if (args.size() != 1) {
        CArray arr;
        size_t size = std::get<double>(args[0]);
        arr.arr.resize(size);
        return arr;
    } else if (args.size() == 2) {
        CArray arr;
        size_t size = std::get<double>(args[0]);
        arr.arr.resize(size, args[1]);
        return arr;
    } else {
        throw std::runtime_error("MakeArray: invalid number of arguments");
    }
    return CArray{};
}



