#include "sort.h"
#include "visitor.h"
#include "truth_checker.h"

#include <algorithm>



Object Sort::Execute() {
    return std::visit(*this, args[0]);
}

Object Sort::operator()(CArray& a) {
    std::sort(a.arr.begin(), a.arr.end(), [](Object& a, Object& b) {
        return std::visit(TruthChecker{}, std::visit(LessVisitor{}, a, b));
    });
    return a;
}