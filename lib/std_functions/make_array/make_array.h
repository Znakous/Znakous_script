#pragma once

#include <std_functions/base_std.h>
#include <object.h>


struct MakeArray : BaseStd {
    ~MakeArray() = default;
    Object Execute();
    static constexpr std::string_view name = "make_array";
};
