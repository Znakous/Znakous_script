// add: PrintLn

#pragma once
#include "object.h"
#include <std_functions/base_std.h>

struct PrintLn : BaseStd {
    PrintLn() : BaseStd(std::cout) {}
    PrintLn(std::ostream& out, std::vector<Object>&& args_);
    ~PrintLn() = default;
    template<typename T>
    void operator()(T& a) {
        out->get() << a;
    };
    void operator()(CArray& a);
    void operator()(StdFuncPtr& a);
    Object Execute();
    
    static constexpr std::string_view name = "println";
};