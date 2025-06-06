#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>




TEST(ClosureTests, EarlyReturnAndClosureTest) {
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

TEST(ClosureTests, NestedClosureTest) {
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

TEST(ClosureTests, ClosureRecursionTest) {
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
        
        fact = makeFactorial(13)
        r = print(fact(5))
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "120");
}

