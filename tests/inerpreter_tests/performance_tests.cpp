
#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(PerformanceTests, LongWhileTest) {
    std::ostringstream test;
    std::string code = R"(
    a = 0
    while a < 1000000
        a = a + 1
    endwhile
    print(a)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1e+06");
}

TEST(PerformanceTests, LongRecursionTest) {
    std::ostringstream test;
    std::string code = R"(
    ident = function(n)
        if n == 0
            return 0
        endif
        return ident(n-1) + 1
    endfunction
    print(ident(100))
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "100");
}