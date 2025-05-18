#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>




TEST(LexerTests, SimpleTest) {
    std::string code = R"(function "chumba-umba" 567 + 1e45 - 14e-8 / <=)";
    Preprocessor prep(code);

    std::cout << "do lexer\n";
    Lexer lex(prep.GetCode());
    std::cout << "constructed lexer\n";
    std::vector<Token> tokens;
    std::vector<Token> tokens_exp{{TokenType::func, std::nullopt}, {TokenType::stringliteral, "chumba-umba"}, {TokenType::intliteral, "567"}, {TokenType::plus, std::nullopt}, {TokenType::intliteral, "1e45"}, {TokenType::minus, std::nullopt}, {TokenType::intliteral, "14e-8"}, {TokenType::divide, std::nullopt}, {TokenType::leq, std::nullopt}};
    while(auto ans = lex.GetToken()) {
        tokens.push_back(ans.value());
        std::cout << (int) ans.value().type << "\n";
        if (ans.value().type == TokenType::stringliteral) {
            std::cout << "val: " << ans.value().value.value() << "\n";
        }
        if (ans.value().type == TokenType::intliteral) {
            std::cout << "valint: " << ans.value().value.value() << "\n";
        }
    }
    ASSERT_THAT(tokens, testing::ElementsAreArray(tokens_exp));
}