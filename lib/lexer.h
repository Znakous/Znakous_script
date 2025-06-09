#pragma once

#include <cstdint>
#include <cctype>
#include <string_view>
#include <expected>

#include "token.h"
#include "trie.h"
#include "logger.h"


class Lexer {
public:
    Lexer(const char* data, std::shared_ptr<logging::Logger> logger);
    Lexer(std::istream& in, std::shared_ptr<logging::Logger> logger);
    bool IsIdentSymbol();
    void ReadOne();
    Token PeekToken();
    Token GetToken();
private:
    using istream_ref = std::reference_wrapper<std::istream>;
    void FindNext(char symbol);
    void FindNumber(bool met_e, bool e_was_last);

    Token NextToken();

    std::string str_;
    size_t derived_size;
    uint32_t cur_pos_;
    uint32_t read_pos_;
    char cur_char_;
    Trie<Token> keywords_;
    Trie<Token> identifiers_;
    Token cur_token_;
    std::shared_ptr<logging::Logger> logger_;
    std::optional<istream_ref> in_;
};