#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(SlicesTests, SimpleTest) {
    std::ostringstream test;
    std::string code = R"(
        a = [1, 2, 3, 4, 5]
        b = a[1:3]
        print(b[0])
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "2");
}


TEST(SlicesTests, OutSliceTest) {
    std::ostringstream test;
    std::string code = R"(
        a = [1, 2, 3, 4, 5]
        b = a[1:3]
        print(b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "[2, 3]");
}
TEST(SlicesTests, StepTest) {
    std::ostringstream test;
    std::string code = R"(
        a = [1, 2, 3, 4, 5]
        b = a[1:5:2]
        print(b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "[2, 4]");
}
TEST(SlicesTests, BackTest) {
    std::ostringstream test;
    std::string code = R"(
        a = [1, 2, 3, 4, 5]
        b = a[:3]
        print(b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "[1, 2, 3]");
}
TEST(SlicesTests, BackStepTest) {
    std::ostringstream test;
    std::string code = R"(
        a = [1, 2, 3, 4, 5]
        b = a[:3:2]
        print(b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "[1, 3]");
}
