#include "print.h"

void Print::operator()(CArray& a) {
    out->get() << "[";
    for (size_t i = 0; i < a.arr.size(); ++i) {
        std::visit(*this, a.arr[i]);
        if (i != a.arr.size() - 1) {
            out->get() << ", ";
        }
    }
    out->get() << "]";
}
void Print::operator()(StdFuncPtr& a) {
    out->get() << "<std_function> ";
}
Object Print::Execute() { 
    for (auto& arg : args) {
        std::visit(*this, arg);
    }
    return CNull{};
}
