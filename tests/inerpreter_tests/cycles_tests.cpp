#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>



TEST(CyclesTests, WhileTest) {
    std::ostringstream test;
    std::string code = R"(
        i = 0
        while (i - 10)
            i = i + 1
        endwhile
        print (i)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "10");
}   

