// test_try_gtest.cpp
#include <gtest/gtest.h>  // Google Test header
#include "try_gtest.h"     // Include the header where the function is declared

// Define a test case using Google Test
TEST(TryGTestTest, one) {
    // Test that try_gtest_func1() returns 5
    EXPECT_EQ(try_gtest_func1(), 5);  // This checks if the function returns 5
}
