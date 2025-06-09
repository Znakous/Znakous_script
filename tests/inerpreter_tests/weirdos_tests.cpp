#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(WeirdosTests, LiteralETest) {
    std::ostringstream test;
    std::string code = R"(
        print(1e30)
        print(1e+30)
        print(1e-30)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1e+301e+301e-30");
}