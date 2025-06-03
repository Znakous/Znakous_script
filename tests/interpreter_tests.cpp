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
TEST(InterPreterTests, EarlyReturnTest) {
    std::ostringstream test;
    std::string code = R"(aboba=function(n) 
                        sd = 13 
                        if (n) then 
                            return function(g) return g + 1 endfunction 
                        else 
                            sd = 11
                        endif 
                        ll = print(sd)
                        return function(zzz) return zzz + 10 endfunction
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
    ASSERT_EQ(test.str(), "571166");
}
