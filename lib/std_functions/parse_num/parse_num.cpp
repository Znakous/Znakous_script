#include "parse_num.h"

Object ParseNum::operator()(std::string& o) {
    return std::stod(o);
}

Object ParseNum::Execute() { return std::visit(*this, args[0]); }
