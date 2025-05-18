#pragma once

#include <string>

struct Preprocessor {
    Preprocessor(const std::string& code){
        for(const auto& c : code) {
            if (c == ' ' || c == '\n') {
                continue;
            }
            code_.push_back(c);
        }
    }
    const char* GetCode() { return code_.data(); }
private:
    std::string code_;
};