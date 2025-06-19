#include <lib/interpreter.h>
#include <gtest/gtest.h>

TEST(RealProgramsTests, CountOnes) {
    std::ostringstream test;
    std::string code = R"(
        count_ones = function(arr, n)
            count = 0
            i = 0
            while i < n
                if arr[i] == 1
                    count = count + 1
                endif
                i = i + 1
            endwhile
            return count
        endfunction

        arr = [1, 0, 1, 1, 0, 1, 0, 1]
        n = 8
        print(count_ones(arr, n))
    )";

    std::string expected = "5";

    std::istringstream input(code);

    Interpreter p(code, test, false);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), expected);
}

TEST(RealProgramsTests, Fibonacci) {
    std::ostringstream test;
    std::string code = R"(
        fibonacci = function(n)
            if n <= 1
                return n
            endif
            return fibonacci(n-1) + fibonacci(n-2)
        endfunction

        print(fibonacci(10))
    )";

    std::string expected = "55";

    std::istringstream input(code);

    Interpreter p(code, test, false);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), expected);
}   

TEST(RealProgramsTests, Factorial) {
    std::ostringstream test;
    std::string code = R"(
        incr = function(n)
            return n+1
        endfunction

        incr_n_times = function(n, k)
            if k == 0
                return n
            endif
            return incr(incr_n_times(n, k-1))
        endfunction

        mult = function(a, multer)
            return a*multer
        endfunction

        print(mult(incr_n_times(10, 2), 4))
    )";

    std::string expected = "48";

    std::istringstream input(code);

    Interpreter p(code, test, false);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), expected);
}