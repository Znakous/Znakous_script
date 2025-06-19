#include "make_array.h"
#include <stdexcept>

Object MakeArray::Execute() {
    if (args.size() != 1) {
        CArray arr;
        size_t size = std::get<double>(args[0]);
        arr.arr.resize(size);
        return arr;
    } else if (args.size() == 2) {
        CArray arr;
        size_t size = std::get<double>(args[0]);
        double value = std::get<double>(args[1]);
        arr.arr.resize(size, value);
        return arr;
    } else {
        throw std::runtime_error("MakeArray: invalid number of arguments");
    }
    return CArray{};
}



