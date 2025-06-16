#include "std_func_spec.h"


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


StdFuncPtr GetStdFunc(std::string_view s, std::ostream& out, std::vector<Object>&& args) {
    if (s == "print") {
        return std::make_shared<StdFunc>(Print{out, std::move(args)});
    } else if (s == "parse_num") {
        return std::make_shared<StdFunc>(ParseNum{out, std::move(args)});
    } else if (s == "abs") {
        return std::make_shared<StdFunc>(Abs{out, std::move(args)});
    }
    throw std::runtime_error("Unknown standard function: " + std::string(s));
}

StdFuncGetter::StdFuncGetter(){
    LIST_OF_FUNCS_TO_INSERT(functions_);
}

StdFuncPtr StdFuncGetter::GetStdFunc(std::string_view s, std::ostream& out, std::vector<Object>&& args) {
    auto resp = functions_.get(s.data());
    if (!resp) {
        throw std::runtime_error("Unknown standard function: " + std::string(s));
    }
    auto func = resp.value().param;
    std::visit(OutSetter{out}, func);
    std::visit(ArgsSetter{std::move(args)}, func);
    return std::make_shared<StdFunc>(std::move(func));
}



