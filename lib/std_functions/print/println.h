// add: PrintLn

#pragma once
#include "object.h"
#include <std_functions/base_std.h>

struct PrintLn : BaseStd {
    ~PrintLn() = default;
    template<typename T>
    void operator()(const T& a) {
        out->get() << a;
    };
    void operator()(const CArray& a);
    void operator()(const StdFuncPtr& a);
    Object Execute();
    
    static constexpr std::string_view name = "println";
};