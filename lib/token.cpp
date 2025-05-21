#include "token.h"


bool IsPrefixOperator(TokenType type) {
    return (type == TokenType::plus) || (type == TokenType::minus);
}

template<size_t Level>
bool IsLevelOperator(Token token) = delete;

template<>
bool IsLevelOperator<1>(Token token) {
    return (token.type == TokenType::less) || (token.type == TokenType::greater)
        || (token.type == TokenType::leq) || (token.type == TokenType::geq);
}

template<>
bool IsLevelOperator<2>(Token token) {
    return (token.type == TokenType::plus) || (token.type == TokenType::minus);
}

template<>
bool IsLevelOperator<3>(Token token) {
    return (token.type == TokenType::multiply) || (token.type == TokenType::divide);
}

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
