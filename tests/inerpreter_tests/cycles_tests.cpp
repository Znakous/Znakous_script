#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>



// TEST(CyclesTests, WhileTest) {
//     std::ostringstream test;
//     std::string code = R"(
//         i = 0
//         while (i - 10)
//             i = i + 1
//         endwhile
//         print (i)
//     )";
//     Interpreter p(code, test);
//     auto el = p.ParseProgram();
//     p.Evaluate(el);
//     ASSERT_EQ(test.str(), "10");
// }   
TEST(CyclesTests, NestedWhileTest) {
    std::ostringstream test;
    std::string code = R"(
        i = 0

        while (i - 10)
            print (i)
            j = 0
            while (j - 2)
                j = j + 1
                print (j)
            endwhile
            i = i + 1
        endwhile
       
        print (i)
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "01211221231241251261271281291210");
}   

TEST(CyclesTests, WhileBreakTest) {
    std::ostringstream test;
    std::string code = R"(
        i = 0
        while (1)
            i = i + 1
            if ((i - 5))
                7
            else 
                break
            endif
            print(i)
        endwhile
        print("done")
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1234done");
}

TEST(CyclesTests, WhileContinueTest) {
    std::ostringstream test; 
    std::string code = R"(
        i = 0
        while (i - 5)
            i = i + 1
            if (i - 3)
                continue
            endif
            print(i)
        endwhile
        print("end")
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "3end");
}

TEST(CyclesTests, NestedWhileBreakTest) {
    std::ostringstream test;
    std::string code = R"(
        i = 0
        while (i - 3)
            i = i + 1
            j = 0
            while (j - 4)
                j = j + 1
                if (j - 3)
                    7
                else break
                endif
                print(j)
            endwhile
            print(i)
        endwhile
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "121122123");
}

