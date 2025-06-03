#pragma once

#include <optional>
#include <string_view>
#include <unordered_map>

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
    for_s, while_s, in_s,
    assign, endif, comma, endfunc,
    stdfunc, endwhile
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
    {"while", {TokenType::while_s, std::nullopt}},
    {"endwhile", {TokenType::endwhile, std::nullopt}},
    {"in", {TokenType::in_s, std::nullopt}},
    {"(", {TokenType::lparen, std::nullopt}},
    {")", {TokenType::rparen, std::nullopt}},
    {"{", {TokenType::lbrace, std::nullopt}},
    {"}", {TokenType::rbrace, std::nullopt}},
    {"=", {TokenType::assign, std::nullopt}},
    {"+", {TokenType::plus, std::nullopt}},
    {"-", {TokenType::minus, std::nullopt}},
    {"*", {TokenType::multiply, std::nullopt}},
    {"/", {TokenType::divide, std::nullopt}},
    {"%", {TokenType::mod, std::nullopt}},
    {"==", {TokenType::eq, std::nullopt}},
    {"<", {TokenType::less, std::nullopt}},
    {">", {TokenType::greater, std::nullopt}},
    {"<=", {TokenType::leq, std::nullopt}},
    {">=", {TokenType::geq, std::nullopt}},
    {",", {TokenType::comma, std::nullopt}},
    {"true", {TokenType::intliteral, "1"}},
    {"false", {TokenType::intliteral, "0"}},
};
constexpr std::string_view std_funcs[] = {
    "print",
    "parsenum"
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
// constexpr bool allowed_in_identifier[255];

// struct IdentLookup {
//     IdentLookup() : table_({
        

        
//     }) {}

//     bool Contains(std::string_view value) {
//         return table_.contains(value);
//     }
//     void Add(std::string_view value, TokenType type) {
//         table_[value] = type;
//     }
//     std::unordered_map<std::string_view, TokenType> table_ ;
// };
