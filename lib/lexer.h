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
    Lexer(std::string&& data, std::shared_ptr<logging::Logger> logger);
    virtual ~Lexer() = default;
    bool IsIdentSymbol();
    virtual void ReadOne() = 0;
    Token PeekToken();
    Token GetToken();
    TokenType GetFunnyAssign(std::string_view op);
protected:
    void FindNext(char symbol);
    void FindNumber(bool met_e, bool e_was_last);
    virtual Token NextToken() = 0;

    std::string str_;
    size_t derived_size;
    uint32_t cur_pos_;
    uint32_t read_pos_;
    char cur_char_;
    Trie<Token> keywords_;
    Trie<Token> identifiers_;
    Token cur_token_;
    std::shared_ptr<logging::Logger> logger_;
};

class NormalLexer : public Lexer {
public:
    NormalLexer(const char* data, std::shared_ptr<logging::Logger> logger);
    NormalLexer(std::istream& in, std::shared_ptr<logging::Logger> logger);
    void ReadOne() override;
    Token NextToken() override;
private:
    std::string_view GetIdent();
};

class WildLexer : public Lexer {
public:
    WildLexer(const char* data, std::shared_ptr<logging::Logger> logger);
    WildLexer(std::istream& in, std::shared_ptr<logging::Logger> logger);
    void ReadOne() override;
    Token NextToken() override;
};
