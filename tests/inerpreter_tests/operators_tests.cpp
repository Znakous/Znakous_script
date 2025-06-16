
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
    if a + 7 > 3 then
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

TEST(OperatorsTests, ModTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 10
    b = 3
    print(a % b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1");
}

TEST(OperatorsTests, PowerTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 2
    b = 3
    print(a ^ b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "8");
}

TEST(OperatorsTests, PowPrecedenceTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 2 ^ 3 + 4 ^ 2
    b = 3 ^ 2 + 4 ^ 2
    print(a, b)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "2425");
}

TEST(OperatorsTests, BigPrecedenceTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 2 ^ 3 + 6 * (5 - 2 ^ 7) * 1 - 2 ^ 3
    print(a)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "-738");
}

TEST(OperatorsTests, FunnyAssignTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 2
    a += 3
    print(a)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "5");
}

TEST(OperatorsTests, BigFunnyAssignTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 2
    a += 3
    a *= 14
    a /= 7
    a %= 7
    print(a)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "3");
}

TEST(OperatorsTests, FunnyAssignArrayTest) {
    std::ostringstream test;
    std::string code = R"(
    a = [1, 2, 3]
    a[0] += 4
    print(a[0])
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "5");
}

TEST(OperatorsTests, MultipleFunnyAssignArrayTest) {
    std::ostringstream test;
    std::string code = R"(
    a = [1, 2, 3]
    a[0] += 4
    a[0] /= 5
    a[0] *= 6
    print(a[0])
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "6");
}
