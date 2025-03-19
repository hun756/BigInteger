#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>
#include <tuple>

TEST(ArithmeticOperationsTest, ExtendedGCD)
{
    using namespace Numerics::detail;

    auto [gcd1, x1, y1] = ArithmeticOperations::extended_gcd<int>(35, 8);
    EXPECT_EQ(gcd1, 1);
    EXPECT_EQ(35 * x1 + 8 * y1, gcd1);

    auto [gcd2, x2, y2] = ArithmeticOperations::extended_gcd<int>(12, 18);
    EXPECT_EQ(gcd2, 6);
    EXPECT_EQ(12 * x2 + 18 * y2, gcd2);

    auto [gcd3, x3, y3] = ArithmeticOperations::extended_gcd<int>(20, 5);
    EXPECT_EQ(gcd3, 5);
    EXPECT_EQ(20 * x3 + 5 * y3, gcd3);

    auto [gcd4, x4, y4] = ArithmeticOperations::extended_gcd<int>(240, 46);
    EXPECT_EQ(gcd4, 2);
    EXPECT_EQ(240 * x4 + 46 * y4, gcd4);

    auto [gcd5, x5, y5] = ArithmeticOperations::extended_gcd<int>(-15, 25);
    EXPECT_EQ(gcd5, -5);
    EXPECT_EQ(-15 * x5 + 25 * y5, gcd5);

    EXPECT_EQ(std::abs(gcd5), 5);

    auto [gcd6, x6, y6] = ArithmeticOperations::extended_gcd<int>(17, 0);
    EXPECT_EQ(gcd6, 17);
    EXPECT_EQ(x6, 1);
    EXPECT_EQ(y6, 0);

    auto [gcd7, x7, y7] = ArithmeticOperations::extended_gcd<int64_t>(123456789, 987654321);
    EXPECT_EQ(gcd7, 9);
    EXPECT_EQ(123456789 * x7 + 987654321 * y7, gcd7);
}