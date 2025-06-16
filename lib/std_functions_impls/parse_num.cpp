#include "parse_num.h"


ParseNum::ParseNum(std::ostream& out, std::vector<Object>&& args_)
    : out(out), args(std::move(args_))
{}

Object ParseNum::operator()(std::string& o) {
    return std::stod(o);
}

Object ParseNum::Execute() { return std::visit(*this, args[0]); }
