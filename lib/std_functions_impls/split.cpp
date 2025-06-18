#include "split.h"

Object Split::Execute() {
    return std::visit(*this, args[0], args[1]);
}

Object Split::operator()(std::string& a, std::string& b) {
    if (b.size() != 1) {
        throw std::runtime_error("Split: separator must be a single character");
    }
    char sep = b[0];
    CArray ans;
    std::string cur_string = "";
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] == sep) {
            ans.arr.push_back(cur_string);
            cur_string = "";
        } else {
            cur_string += a[i];
        }
    }
    if (!cur_string.empty()) {
        ans.arr.push_back(cur_string);
    }
    return ans;
}
