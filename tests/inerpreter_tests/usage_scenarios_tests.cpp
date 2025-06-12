#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(UsageScenariosTests, IstreamTest) {
    std::ostringstream test;
    std::string code = R"(
        a = [1, 2, 3, 4, 5]
        print(a[0])
        print(a[4])
        
        b2 = [10, 20, 30]
        print(b2[0])
        
        i = 1
        print(b2[i])
        print(b2[i+1])
    )";
    std::istringstream in(code);
    Interpreter p(in, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "15102030");
}


TEST(UsageScenariosTests, IstreamTest2) {
    std::ostringstream test;
    std::string code = R"(
        funcs = [
            function() return 1 endfunction,
            function() return 2 endfunction,
            function() return 3 endfunction,
        ]

        print(funcs[0]())
        print(funcs[1]())
        print(funcs[2]()))";
    std::istringstream in(code);
    Interpreter p(in, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "123");
}