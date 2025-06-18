#include "to_string.h"
#include <sstream>


Object ToString::Execute() {
    return std::visit(*this, args[0]);
}

Object ToString::operator()(double& arg) {
    std::ostringstream strs;
    strs << arg;
    std::string str = strs.str();
    return str;
}