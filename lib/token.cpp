#include "token.h"


bool IsPrefixOperator(TokenType type) {
    return (type == TokenType::plus) || (type == TokenType::minus);
}

// template<size_t Level>
// bool IsLevelOperator(Token token) = delete;

template<>
bool IsLevelOperator<1>(Token token) {
    return (token.type == TokenType::less) || (token.type == TokenType::greater)
        || (token.type == TokenType::leq) || (token.type == TokenType::geq);
}

template<>
bool IsLevelOperator<2>(Token token) {
    return (token.type == TokenType::power);
}

template<>
bool IsLevelOperator<3>(Token token) {
    return (token.type == TokenType::multiply) || (token.type == TokenType::divide)
        || (token.type == TokenType::mod);
}

template<>
bool IsLevelOperator<4>(Token token) {
    return (token.type == TokenType::plus) || (token.type == TokenType::minus);
}

template<>
bool IsLevelOperator<5>(Token token) {
    return (token.type == TokenType::less) || (token.type == TokenType::greater)
        || (token.type == TokenType::leq) || (token.type == TokenType::geq)
        || (token.type == TokenType::eq) || (token.type == TokenType::neq);
}


// TokenType GetFunnyAssign(std::string_view op) {
//     if (op == "+") return TokenType::plus;
//     else if (op == "-") return TokenType::minus;
//     else if (op == "*") return TokenType::multiply;
//     else if (op == "/") return TokenType::divide;
//     else if (op == "%") return TokenType::mod;
//     else if (op == "^") return TokenType::power;
//     else throw std::runtime_error("Unknown funny assign operator: " + std::string(op));
// }