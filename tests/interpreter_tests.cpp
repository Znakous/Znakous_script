#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>




TEST(InterPreterTests, SimpleTest) {
    std::ostringstream test;
    std::string code = R"(aboba=7+8+(10*3) aboba = aboba + 2 if 0 then aboba = 3 - aboba else aboba = aboba + 1 endif pr = print(aboba))";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "48");
}
TEST(InterPreterTests, FuncTest) {
    std::string code = R"(aboba=function(n, m) return function(g) return g + 1 endfunction endfunction rt = 5 fr = aboba(3, rt) uk = fr(56))";
    Interpreter p(code, std::cout);
    auto el = p.ParseProgram();
    p.Evaluate(el);
}
TEST(InterPreterTests, PrintTest) {
    std::ostringstream test;
    std::string code = R"(aboba=function(n, m) return function(g) return g + 1 endfunction endfunction rt = 5 fr = aboba(3, rt) uk = fr(56) t = print(uk))";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "57");
}
TEST(InterPreterTests, PrintIfTest) {
    std::ostringstream test;
    std::string code = R"(aboba=function(n, m) return function(g) return g + 1 endfunction endfunction rt = 5 fr = aboba(3, rt) uk = fr(56) t = uk if (t - 57) then r = print(44) else r = print("amogus") endif)";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "amogus");
}
TEST(InterPreterTests, EarlyReturnAndClosureTest) {
    std::ostringstream test;
    std::string code = R"(aboba=function(n) 
                        sd = 13 
                        if (n) then 
                            return function(g) return g + 1 endfunction 
                        else 
                            sd = 11
                        endif 
                        ll = print(sd)
                        return function(zzz) return zzz + sd endfunction
                        endfunction 
                        rt = 5 
                        fr = aboba(3) 
                        uk = fr(56) 
                        r = print(uk)
                        vv = aboba(0) 
                        e = print(vv(56))
                        )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "571167");
}

TEST(InterPreterTests, NestedClosureTest) {
    std::ostringstream test;
    std::string code = R"(
        counter = function(start)
            count = start
            return function(i)
                chipi = count + i
                return chipi
            endfunction
        endfunction
        
        c1 = counter(10)
        c2 = c1(1)
        c3 = c1(2)
        r1 = print(c2)
        r2 = print(c3)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1112");
}

TEST(InterPreterTests, ClosureRecursionTest) {
    std::ostringstream test;
    std::string code = R"(
        makeFactorial = function(m)
            factorial = function(n)
                if n then
                    return n * factorial(n - 1)
                else
                    return 1
                endif
            endfunction
            return factorial
        endfunction
        
        ans = makeFactorial(13)
        r = print(ans(5))
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "120");
}
