#pragma once
#include "base_std.h"
#include "object.h"

struct TruthChecker {
    bool operator()(double a);
    bool operator()(const std::string& s);
    bool operator()(CNull);
    bool operator()(BuiltinFuncPtr);
    bool operator()(CArray a);
    bool operator()(const StdFuncPtr& f);
};