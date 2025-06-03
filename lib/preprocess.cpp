#include "usings.h"
#include "preprocess.h"

std::string Preprocess(const std::string& code){
    std::cout << "into prep\n";
    std::string ans;
    for(const auto& c : code) {
        if (c == ' ' || c == '\n') {
            continue;
        }
        ans.push_back(c);
    }
    std::cout << "preped\n";
    return ans;
}