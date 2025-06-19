#include "std_func_spec.h"




StdFuncGetter::StdFuncGetter(){
    auto names = GetNames<StdFuncVar, StdFuncVar>().names;
    for (auto& func : names) {
        StdFunc var = func.value;
        functions_.insert(func.name, var);
    }
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



