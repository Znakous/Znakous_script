#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(StringTests, StringConcatTest) {
    std::ostringstream test;
    std::string code = R"(aboba = "aboba" print(aboba + "amogus"))";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "abobaamogus");
}


TEST(StringTests, StringMultiplyTest) {
    std::ostringstream test;
    std::string code = R"(aboba = "aboba" print(aboba * 3))";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "abobaabobaaboba");
}

TEST(StringTests, StringSubtractTest) {
    std::ostringstream test;
    std::string code = R"(
        str1 = "helloworld"
        str2 = "world"
        print(str1 - str2)
        
        str3 = "testing"
        str4 = "xyz"
        print(str3 - str4)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "hellotesting");
}


TEST(StringTests, StringDivideTest) {
    std::ostringstream test;
    std::string code = R"(
        str1 = "helloworld"
        print(str1 / 2)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "hello");
}


TEST(StringTests, StringReverseTest) {
    std::ostringstream test;
    std::string code = R"(
        str1 = "helloworld"
        print(-str1)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "dlrowolleh");
}
TEST(StringTests, StringDoubleReverseTest) {
    std::ostringstream test;
    std::string code = R"(
        str1 = "helloworld"
        print(-(-str1))
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "helloworld");
}
TEST(StringTests, StringTripleReverseTest) {
    std::ostringstream test;
    std::string code = R"(
        str1 = "helloworld"
        print(-(-(-str1)))
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "dlrowolleh");
}
TEST(StringTests, ComplexStringOperationsTest) {
    std::ostringstream test;
    std::string code = R"(
        print(-("hola" * 4)/2 + "test")
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
        ASSERT_EQ(test.str(), "alohalohtest");
}

