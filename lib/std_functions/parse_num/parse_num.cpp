#include "parse_num.h"

Object ParseNum::operator()(const std::string& o) {
    return std::stod(o);
}

Object ParseNum::Execute() { 
    if (args.size() != 1) {
        throw std::runtime_error(
            "ParseNum: invalid number of arguments, expected 1, got " + std::to_string(args.size())
        );
    }
    return std::visit(*this, args[0]); 
}
