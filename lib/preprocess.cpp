#include "usings.h"
#include "preprocess.h"
#include "lexer_helpers.h"

std::string PreprocessWild(const std::string& code){
    std::string ans;
    for(const auto& c : code) {
        if (c == ' ' || c == '\n') {
            continue;
        }
        ans.push_back(c);
    }
    return ans;
}

std::string PreprocessNormal(const std::string& code){
    std::string ans;
    bool deleting = false;
    for(const auto& c : code) {
        if (c == ' ' || c == '\n') {
            if (deleting) {
                continue;
            } else {
                deleting = true;
                ans.push_back(' ');
            }
        } else {
            deleting = false;
            ans.push_back(c);
        }
    }
    return ans;
    
}

std::string PreprocessWild(std::istream& in){
    std::string ans;
    char buffer;
    while (in.get(buffer)) {
        
        if (!is_whitespace(buffer)) {
            ans.push_back(buffer);
        }
    }
    return ans;
}

std::string PreprocessNormal(std::istream& in){
    std::string ans;
    char buffer;
    
    while (in.get(buffer)) {
        if (!is_whitespace(buffer)) {
            ans.push_back(buffer);
        } else {
            ans.push_back(' ');
            while (in.get(buffer) && is_whitespace(buffer)) {}
            if (!in.eof()) {
                ans.push_back(buffer);
            }
        }
    }
    return ans;
}