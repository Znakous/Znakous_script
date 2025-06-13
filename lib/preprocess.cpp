#include "usings.h"
#include "preprocess.h"
#include "lexer_helpers.h"

struct CommentEraser {
    CommentEraser()
     : last_symbol('\0'), erasing_block(false), erasing_line(false) 
    {}
    void Push(char symbol, std::string& ans) {
        bool erasing = erasing_block || erasing_line;
        Move(symbol);
        last_symbol = symbol;
        if (!erasing) {
            ans.push_back(symbol);
        }
    }
    void Move(char symbol) {
        if (erasing_block) {
            if (symbol == '/' && last_symbol == '*') {
                erasing_block = false;
            }
        } else if (erasing_line) {
            if (symbol == '\n') {
                erasing_line = false;
            }
        } else {
            if (symbol == '*' && last_symbol == '/') {
                erasing_block = true;
            } else if (symbol == '/' && last_symbol == '/') {
                erasing_line = true;
            }
        }
        last_symbol = symbol;
    }
    char last_symbol;
    bool erasing_block;
    bool erasing_line;
};

std::string PreprocessWild(const std::string& code){
    std::string ans;
    CommentEraser eraser;
    for(const auto& c : code) {
        if (c == ' ' || c == '\n') {
            continue;
        }
        eraser.Push(c, ans);
    }
    return ans;
}

std::string PreprocessNormal(const std::string& code){
    std::string ans;
    bool deleting = false;
    CommentEraser eraser;
    for(const auto& c : code) {
        if (c == ' ' || c == '\n') {
            if (deleting) {
                continue;
            } else {
                deleting = true;
                eraser.Push(' ', ans);
            }
        } else {
            deleting = false;
            eraser.Push(c, ans);
        }
    }
    return ans;
    
}

std::string PreprocessWild(std::istream& in){
    std::string ans;
    char buffer;
    CommentEraser eraser;
    while (in.get(buffer)) {
        
        if (!is_whitespace(buffer)) {
            eraser.Push(buffer, ans);
        }
    }
    return ans;
}

std::string PreprocessNormal(std::istream& in){
    std::string ans;
    char buffer;
    CommentEraser eraser;
    while (in.get(buffer)) {
        if (!is_whitespace(buffer)) {
            eraser.Push(buffer, ans);
        } else {
            eraser.Push(' ', ans);
            while (in.get(buffer) && is_whitespace(buffer)) {}
            if (!in.eof()) {
                eraser.Push(buffer, ans);
            }
        }
    }
    return ans;
}