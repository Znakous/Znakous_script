#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(WildTests, SimpleTest) {
    std::ostringstream test;
    std::string code = R"(number=56number=number+1print(number))";
    Interpreter p(code, test, true);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "57");
}


TEST(WildTests, SimpleTest2) {
    std::ostringstream test;
    std::string code = R"(number=56number=number+1ifnumber-50thenprint(number)elseprint(number-1)endif)";
    Interpreter p(code, test, true);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "57");
}
