#include "replace.h"

Object Replace::Execute() {
    return std::visit(*this, args[0], args[1], args[2]);
}

Object Replace::operator()(std::string& a, std::string& old, std::string& new_) {
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
