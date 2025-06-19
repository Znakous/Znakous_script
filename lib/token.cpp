#include "token.h"


bool IsPrefixOperator(TokenType type) {
    return (type == TokenType::plus) || (type == TokenType::minus);
}

template<>
bool IsLevelOperator<2>(Token token) {
    return (token.type == TokenType::bit_and) || (token.type == TokenType::bit_or)
        || (token.type == TokenType::bit_xor);
}

template<>
bool IsLevelOperator<3>(Token token) {
    return (token.type == TokenType::power);
}

template<>
bool IsLevelOperator<4>(Token token) {
    return (token.type == TokenType::multiply) || (token.type == TokenType::divide)
    || (token.type == TokenType::mod);
}

template<>
bool IsLevelOperator<5>(Token token) {
    return (token.type == TokenType::plus) || (token.type == TokenType::minus);
}

template<>
bool IsLevelOperator<6>(Token token) {
    return (token.type == TokenType::less) || (token.type == TokenType::greater)
    || (token.type == TokenType::leq) || (token.type == TokenType::geq)
    || (token.type == TokenType::eq) || (token.type == TokenType::neq);
}
template<>
bool IsLevelOperator<7>(Token token) {
    return (token.type == TokenType::and_)
        || (token.type == TokenType::or_)
        || (token.type == TokenType::xor_);
}

