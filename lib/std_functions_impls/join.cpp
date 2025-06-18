#include "join.h"
#include "visitor.h"

Object Join::Execute() {
    return std::visit(*this, args[0], args[1]);
}

Object Join::operator()(CArray& a, std::string& b_str) {
    if (a.arr.empty()) {
        return "";
    }
    Object ans = "";
    Object b = b_str;
    for (size_t i = 0; i < a.arr.size()-1; i++) {
        ans = std::visit(AddVisitor{}, ans, a.arr[i]);
        ans = std::visit(AddVisitor{}, ans, b);
    }
    ans = std::visit(AddVisitor{}, ans, a.arr.back());
    return ans;
}