
#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(OperatorsTests, IfTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 5
    b = 1
    if a > 3 then
        b = a + 3
    else
        b = a-1
    endif
    print(b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "8");
}

TEST(OperatorsTests, IfLessTest) {
    std::ostringstream test;
    std::string code = R"(
    a = -6
    b = 1
    if a+7 > 3 then
        b = b + 3
    else
        b = b-1
    endif
    print(b)
    a=0
    if a+7 > 3 then
        b = b + 3
    else
        b = b-1
    endif
    print(b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "03");
}