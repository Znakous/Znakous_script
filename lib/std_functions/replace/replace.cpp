#include "replace.h"

Object Replace::Execute() {
    if (args.size() != 3) {
        throw std::runtime_error(
            "Replace: invalid number of arguments, expected 3, got " + std::to_string(args.size())
        );
    }
    for (size_t i = 0; i < 3; ++i) {
        if (!std::holds_alternative<std::string>(args[i])) {
            throw std::runtime_error("Replace: argument " + std::to_string(i) + " must be a string");
        }
    }

    return std::visit(*this, args[0], args[1], args[2]);
}

Object Replace::operator()(const std::string& a, const std::string& old, const std::string& new_) {
    std::string ans;
    size_t pos = 0;
    while (pos+old.size() <= a.size()) {
        if (a.substr(pos, old.size()) == old) {
            ans += new_;
            pos += old.size();
        } else {
            ans += a[pos];
            pos++;
        }
    }
    ans += a.substr(pos);
    return ans;
}
