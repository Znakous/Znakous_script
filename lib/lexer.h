#pragma once

#include <cstdint>
#include <cctype>
#include <string_view>
#include <expected>

#include "token.h"
#include "trie.h"



class Lexer {
public:
    Lexer(const char* data);

    void ReadOne();
    Token PeekToken();
    Token GetToken();
private:
    void FindNext(char symbol);
    void FindNumber(bool met_e, bool e_was_last);

    Token NextToken();

    const char* data_;
    size_t derived_size;
    uint32_t cur_pos_;
    uint32_t read_pos_;
    char cur_char_;
    Trie<Token> keywords_;
    Token for_peeks_;
};