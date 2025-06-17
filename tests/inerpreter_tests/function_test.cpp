#include <lib/interpreter.h>
#include <gtest/gtest.h>

TEST(FunctionTests, SimpleFunctionTest) {
    std::string code = R"(
        incr = function(value)
            return value + 1
        endfunction

        x = incr(2)
        print(x)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream test;

    Interpreter p(code, test, false);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), expected);
}

