#include "println.h"


void PrintLn::operator()(CArray& a) {
    out->get() << "[";
    for (size_t i = 0; i < a.arr.size(); ++i) {
        std::visit(*this, a.arr[i]);
        if (i != a.arr.size() - 1) {
            out->get() << ", ";
        }
    }
    out->get() << "]";
}
void PrintLn::operator()(StdFuncPtr& a) {
    out->get() << "<std_function>";
}
Object PrintLn::Execute() { 
    for (auto& arg : args) {
        std::visit(*this, arg);
    }
    out->get() << "\n";
    return CNull{};
}
