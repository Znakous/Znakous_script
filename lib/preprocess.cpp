#include "usings.h"
#include "preprocess.h"
#include "lexer_helpers.h"

class CommentEraser {
public:
    CommentEraser() : last_symbol('\0'), in_string(false) {}

    void Push(char symbol, std::string& ans) {
        if (symbol == '"') {
            in_string = !in_string;
            ans.push_back(symbol);
            last_symbol = symbol;
            return;
        }

        if (in_string) {
            ans.push_back(symbol);
            last_symbol = symbol;
            return;
        }

        if (in_block_comment) {
            if (symbol == '/' && last_symbol == '*') {
                in_block_comment = false;
                if (!ans.empty()) {
                    ans.pop_back();
                }
            }
            last_symbol = symbol;
            return;
        }

        if (in_line_comment) {
            if (symbol == '\n') {
                in_line_comment = false;
            }
            last_symbol = symbol;
            return;
        }

        if (last_symbol == '/') {
            if (symbol == '/') {
                in_line_comment = true;
                if (!ans.empty()) {
                    ans.pop_back();
                }
            } else if (symbol == '*') {
                in_block_comment = true;
                if (!ans.empty()) {
                    ans.pop_back();
                }
            } else {
                ans.push_back(symbol);
            }
        } else {
            ans.push_back(symbol);
        }

        last_symbol = symbol;
    }

private:
    char last_symbol;
    bool in_string;
    bool in_block_comment{false};
    bool in_line_comment{false};
};

std::string PreprocessWild(const std::string& code) {
    std::string ans;
    CommentEraser eraser;
    bool last_was_space = false;

    for (char c : code) {
        if (std::isspace(c)) {
            if (!last_was_space) {
                eraser.Push(' ', ans);
                last_was_space = true;
            }
        } else {
            last_was_space = false;
            eraser.Push(c, ans);
        }
    }
    return ans;
}

std::string PreprocessNormal(const std::string& code) {
    std::string ans;
    CommentEraser eraser;
    bool last_was_space = false;

    for (char c : code) {
        if (std::isspace(c)) {
            if (!last_was_space) {
                eraser.Push(' ', ans);
                last_was_space = true;
            }
        } else {
            last_was_space = false;
            eraser.Push(c, ans);
        }
    }
    return ans;
}

std::string PreprocessWild(std::istream& in) {
    std::string ans;
    CommentEraser eraser;
    bool last_was_space = false;
    char c;

    while (in.get(c)) {
        if (std::isspace(c)) {
            if (!last_was_space) {
                eraser.Push(' ', ans);
                last_was_space = true;
            }
        } else {
            last_was_space = false;
            eraser.Push(c, ans);
        }
    }
    return ans;
}

std::string PreprocessNormal(std::istream& in) {
    std::string ans;
    CommentEraser eraser;
    bool last_was_space = false;
    char c;

    while (in.get(c)) {
        if (std::isspace(c)) {
            if (!last_was_space) {
                eraser.Push(' ', ans);
                last_was_space = true;
            }
        } else {
            last_was_space = false;
            eraser.Push(c, ans);
        }
    }
    return ans;
}
