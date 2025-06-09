#include "std_functions.h"



bool TruthChecker::operator()(double a) {
    return a != 0;
}
bool TruthChecker::operator()(const std::string& s) {
    return !s.empty();
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
bool TruthChecker::operator()(const StdFuncPtr& f) {
    return f != nullptr;
}


bool IsStdFunc(std::string_view s) {
    return s == "print" || s == "parse_num";
}

StdFuncPtr GetStdFunc(std::string_view s, std::ostream& out, std::vector<Object>&& args) {
    if (s == "print") {
        return std::make_shared<StdFunc>(Print{out, std::move(args)});
    } else if (s == "parse_num") {
        return std::make_shared<StdFunc>(ParseNum{out, std::move(args)});
    }
    throw std::runtime_error("Unknown standard function: " + std::string(s));
}