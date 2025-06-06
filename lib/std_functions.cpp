#include "std_functions.h"



bool TruthChecker::operator()(double a) {
    return a != 0;
}
bool TruthChecker::operator()(const std::string& s) {
    return !(s.empty());
}
bool TruthChecker::operator()(CNull) {
    return false;
}
bool TruthChecker::operator()(BuiltinFuncPtr) {
    return true;
}
bool TruthChecker::operator()(CArray a) {
    return !a.arr.empty();
}


bool IsStdFunc(std::string_view s) {
    if (s == "print") {
        return true;
    }
}

