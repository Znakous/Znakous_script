#pragma once

#include <optional>
#include <string_view>
#include <unordered_map>
#include <stdexcept>

#include <std_functions/std_func_include.h>

enum class TokenType {
    eof,
    func,
    ret,    
    if_s, elif_s, else_s,
    number,
    plus, minus, multiply, divide, mod,
    eq, neq, less, greater, leq, geq,
    lbrace, rbrace, 
    lparen, rparen,
    intliteral, stringliteral,
    invalid,
    then,
    ident,
    for_s, endfor, while_s, in_s,
    assign, endif, comma, endfunc,
    stdfunc, endwhile,
    lsquare, rsquare,
    break_s,
    continue_s,
    colon,
    slice_access,
    power,
    funny_assign, null,
    bit_and, bit_or, bit_xor, and_, or_, xor_
};


struct Token {
    constexpr Token()
     : type(TokenType::invalid), value(std::nullopt) 
    {}
    constexpr Token(TokenType t, std::optional<std::string_view> val=std::nullopt)
     : type(t), value(val) 
    {}
    TokenType type;
    std::optional<std::string_view> value;
    operator bool() {return type != TokenType::invalid;}
    bool operator==(const Token&) const = default;
};

constexpr std::pair<std::string_view, Token> keywords[] = {
    {"function", {TokenType::func, std::nullopt}},
    {"endfunction", {TokenType::endfunc, std::nullopt}},
    {"return", {TokenType::ret, std::nullopt}},
    {"if", {TokenType::if_s, std::nullopt}},
    {"elif", {TokenType::elif_s, std::nullopt}},
    {"else", {TokenType::else_s, std::nullopt}},
    {"endif", {TokenType::endif, std::nullopt}},
    {"then", {TokenType::then, std::nullopt}},
    {"for", {TokenType::for_s, std::nullopt}},
    {"endfor", {TokenType::endfor, std::nullopt}},
    {"while", {TokenType::while_s, std::nullopt}},
    {"endwhile", {TokenType::endwhile, std::nullopt}},
    {"in", {TokenType::in_s, std::nullopt}},
    {"break", {TokenType::break_s, std::nullopt}},
    {"continue", {TokenType::continue_s, std::nullopt}},
    {"(", {TokenType::lparen, std::nullopt}},
    {")", {TokenType::rparen, std::nullopt}},
    {"{", {TokenType::lbrace, std::nullopt}},
    {"}", {TokenType::rbrace, std::nullopt}},
    {"=", {TokenType::assign, std::nullopt}},
    {"+=", {TokenType::funny_assign, "+"}},
    {"-=", {TokenType::funny_assign, "-"}},
    {"*=", {TokenType::funny_assign, "*"}},
    {"/=", {TokenType::funny_assign, "/"}},
    {"%=", {TokenType::funny_assign, "%"}},
    {"^=", {TokenType::funny_assign, "^"}},
    {"&=", {TokenType::funny_assign, "&"}},
    {"|=", {TokenType::funny_assign, "|"}},
    {"`=", {TokenType::funny_assign, "`"}},
    {"+", {TokenType::plus, std::nullopt}},
    {"-", {TokenType::minus, std::nullopt}},
    {"*", {TokenType::multiply, std::nullopt}},
    {"/", {TokenType::divide, std::nullopt}},
    {"%", {TokenType::mod, std::nullopt}},
    {"^", {TokenType::power, std::nullopt}},
    {"&", {TokenType::bit_and, std::nullopt}},
    {"&&", {TokenType::and_, std::nullopt}},
    {"|", {TokenType::bit_or, std::nullopt}},
    {"||", {TokenType::or_, std::nullopt}},
    {"`", {TokenType::xor_, std::nullopt}},
    {"``", {TokenType::bit_xor, std::nullopt}},
    {"==", {TokenType::eq, std::nullopt}},
    {"<", {TokenType::less, std::nullopt}},
    {">", {TokenType::greater, std::nullopt}},
    {"<=", {TokenType::leq, std::nullopt}},
    {">=", {TokenType::geq, std::nullopt}},
    {",", {TokenType::comma, std::nullopt}},
    {"[", {TokenType::lsquare, std::nullopt}},
    {"]", {TokenType::rsquare, std::nullopt}},
    {":", {TokenType::colon, std::nullopt}},
    {"true", {TokenType::intliteral, "1"}},
    {"false", {TokenType::intliteral, "0"}},
    {"null", {TokenType::null, std::nullopt}},
    {"nil", {TokenType::null, std::nullopt}},
};


bool IsPrefixOperator(TokenType type);

template<size_t Level>
bool IsLevelOperator(Token token) = delete;

template<>
bool IsLevelOperator<1>(Token token);

template<>
bool IsLevelOperator<2>(Token token);

template<>
bool IsLevelOperator<3>(Token token);

template<>
bool IsLevelOperator<4>(Token token);

template<>
bool IsLevelOperator<5>(Token token);

template<>
bool IsLevelOperator<6>(Token token);

// TokenType GetFunnyAssign(std::string_view op);