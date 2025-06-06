#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(InterPreterTests, ArrayTest) {
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
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "15102030");
}


TEST(InterPreterTests, MixedTypeArrayTest) {
    std::ostringstream test;
    std::string code = R"(
        arr = [1, "hello", function(x) return x+1 endfunction, [1, 2, 3], 2e30]
        print(arr[0])
        print(arr[1])
        f = arr[2]
        print(f(5))
        print(arr[3][2])
        print(arr[4])

    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1hello632e+30");
}

TEST(InterPreterTests, ArrayAssignmentTest) {
    std::ostringstream test;
    std::string code = R"(
        arr = [1, 2, 3]
        arr[1] = "modified"
        print(arr[0])
        print(arr[1])
        print(arr[2])
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1modified3");
}
TEST(InterPreterTests, MultiDimensionalArrayTest) {
    std::ostringstream test;
    std::string code = R"(
        arr = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
        arr[1][1] = "modified"
        print(arr[0][0])
        print(arr[1][1]) 
        print(arr[2][2])
        
        arr[0][2] = "changed"
        print(arr[0][2])
        print(arr[1][2])
    )";
    Interpreter p(code, test);
    auto el = p.ParseProgram();
    p.Evaluate(el);
    ASSERT_EQ(test.str(), "1modified9changed6");
}