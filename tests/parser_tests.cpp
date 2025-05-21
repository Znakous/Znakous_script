#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>




TEST(ParserTests, SimpleTest) {
    std::string code = R"(aboba =7)";
    Preprocessor prep(code);

    Parser p(prep.GetCode());
    
}