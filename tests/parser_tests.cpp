#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>




TEST(ParserTests, SimpleTest) {
    std::string code = R"(aboba =7)";
    Preprocessor prep(code);

    Parser p(prep.GetCode());
    auto el = p.ParseProgram();
    
}
TEST(ParserTests, OnePlusOneTest) {
    std::string code = R"(a=1+1)";
    Preprocessor prep(code);

    Parser p(prep.GetCode());
    p.ParseProgram();
    
}