#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(NonWildTests, PriorityAndIfTest) {
    std::ostringstream test;
    std::string code = R"(aboba=7+8+(10*3) aboba = aboba + 2 if 0 then aboba = 3 - aboba else aboba = aboba + 1 endif pr = print(aboba))";
    Interpreter p(code, test, true);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "48");
}
TEST(NonWildTests, PriorityAndIfTest2) {
    std::ostringstream test;
    std::string code = R"(aboba=7+8+(10*3) aboba = aboba + 2 if 0 then aboba = 3 - aboba else aboba = aboba + 1 endif pr = print(aboba))";
    Interpreter p(code, test, false);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "48");
}
TEST(NonWildTests, SimpleFunctionTest) {
    std::ostringstream test;
    std::string code = R"(
        incr = function(value)
            return value + 1
        endfunction

        x = incr(2)
        print(x)
    )";

    Interpreter p(code, test, false);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "3");
}
