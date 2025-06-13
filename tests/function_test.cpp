#include <lib/interpreter.h>
#include <gtest/gtest.h>

TEST(FunctionTestSuite, SimpleFunctionTest) {
    std::string code = R"(
        incr = function(value)
            return value + 1
        endfunction

        x = incr(2)
        print(x)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(FunctionTestSuite, FunctionAsArgTest) {
    std::string code = R"(
        incr = function(value)
            return value + 1
        endfunction

        printresult = function(value, func)
            result = func(value)
            print(result)
        endfunction

        printresult(2, incr)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(FunctionTestSuite, NestedFunctionTest) {
    std::string code = R"(

        incr_and_print = function(value)
            incr = function(value)
                return value + 1
            endfunction

            print(incr(value))
        endfunction

        incr_and_print(2)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


// TEST(FunctionTestSuite, FunnySyntaxTest) {
//     std::string code = R"(
//         funcs = [
//             function() return 1 endfunction,
//             function() return 2 endfunction,
//             function() return 3 endfunction,
//         ]

//         print(funcs[0]())
//         print(funcs[1]())
//         print(funcs[2]())
//     )";

//     std::string expected = "123";

//     std::istringstream input(code);
//     std::ostringstream output;

//     ASSERT_TRUE(interpret(input, output));
//     ASSERT_EQ(output.str(), expected);
// }
