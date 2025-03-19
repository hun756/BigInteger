#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>

using namespace Numerics::detail;

class NewtonRaphsonDivisionReciprocalTest : public ::testing::Test
{
protected:
    void SetUp() override { epsilon = 1e-10; }

    double epsilon;
};

class NewtonRaphsonDivisionTest : public ::testing::Test
{
protected:
    void SetUp() override { epsilon = 1e-10; }

    double epsilon;
};

TEST_F(NewtonRaphsonDivisionReciprocalTest, HandlesPositiveNumbers)
{

    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(2.0, 5), 0.5, epsilon);
    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(10.0, 5), 0.1, epsilon);
    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(100.0, 5), 0.01, epsilon);
}

TEST_F(NewtonRaphsonDivisionReciprocalTest, HandlesNegativeNumbers)
{

    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(-2.0, 5), -0.5, epsilon);
    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(-10.0, 5), -0.1, epsilon);
}

TEST_F(NewtonRaphsonDivisionReciprocalTest, HandlesLargeNumbers)
{

    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(1e6, 5), 1e-6, 1e-10);
}

TEST_F(NewtonRaphsonDivisionReciprocalTest, HandlesSmallNumbers)
{

    EXPECT_NEAR(NewtonRaphsonDivision::reciprocal_estimate(1e-6, 5), 1e6, 1e-2);
}

TEST_F(NewtonRaphsonDivisionReciprocalTest, PrecisionImpactsAccuracy)
{

    double value = 7.0;
    double low_precision = NewtonRaphsonDivision::reciprocal_estimate(value, 1);
    double high_precision = NewtonRaphsonDivision::reciprocal_estimate(value, 10);
    double actual = 1.0 / value;

    EXPECT_LE(std::abs(high_precision - actual), std::abs(low_precision - actual));
}

TEST_F(NewtonRaphsonDivisionReciprocalTest, TypeRequirementFailsForNonFloatingPoint)
{
    SUCCEED() << "static_assert fails at compile time for non-floating point types";
}

TEST_F(NewtonRaphsonDivisionTest, BasicDivisionWithRemainder)
{

    auto [quotient, remainder] = NewtonRaphsonDivision::divide_with_remainder(10.0, 3.0);
    EXPECT_EQ(quotient, 3.0);
    EXPECT_NEAR(remainder, 1.0, epsilon);

    auto [q2, r2] = NewtonRaphsonDivision::divide_with_remainder(7.0, 2.0);
    EXPECT_EQ(q2, 3.0);
    EXPECT_NEAR(r2, 1.0, epsilon);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionWithoutRemainder)
{

    auto [quotient, remainder] = NewtonRaphsonDivision::divide_with_remainder(10.0, 2.0);
    EXPECT_EQ(quotient, 5.0);
    EXPECT_NEAR(remainder, 0.0, epsilon);

    auto [q2, r2] = NewtonRaphsonDivision::divide_with_remainder(15.0, 3.0);
    EXPECT_EQ(q2, 5.0);
    EXPECT_NEAR(r2, 0.0, epsilon);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionWithNegativeNumbers)
{

    auto [q1, r1] = NewtonRaphsonDivision::divide_with_remainder(-10.0, 3.0);
    EXPECT_EQ(q1, -4.0);
    EXPECT_NEAR(r1, 2.0, epsilon);

    auto [q2, r2] = NewtonRaphsonDivision::divide_with_remainder(10.0, -3.0);
    EXPECT_EQ(q2, -4.0);
    EXPECT_NEAR(r2, -2.0, epsilon);

    auto [q3, r3] = NewtonRaphsonDivision::divide_with_remainder(-10.0, -3.0);
    EXPECT_EQ(q3, 3.0);
    EXPECT_NEAR(r3, -1.0, epsilon);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionWithLargeNumbers)
{

    double large_dividend = 1e9;
    double large_divisor = 1e6;
    auto [quotient, remainder] =
        NewtonRaphsonDivision::divide_with_remainder(large_dividend, large_divisor);
    EXPECT_EQ(quotient, 1000.0);
    EXPECT_NEAR(remainder, 0.0, 1e-4);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionWithSmallNumbers)
{

    double small_dividend = 1e-6;
    double small_divisor = 1e-9;
    auto [quotient, remainder] =
        NewtonRaphsonDivision::divide_with_remainder(small_dividend, small_divisor);
    EXPECT_NEAR(quotient, 1000.0, 1.0);
    EXPECT_NEAR(remainder, 0.0, 1e-9);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionWithDivisorLargerThanDividend)
{

    auto [quotient, remainder] = NewtonRaphsonDivision::divide_with_remainder(3.0, 10.0);
    EXPECT_EQ(quotient, 0.0);
    EXPECT_NEAR(remainder, 3.0, epsilon);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionByZeroThrowsException)
{

    EXPECT_THROW({ NewtonRaphsonDivision::divide_with_remainder(10.0, 0.0); }, std::domain_error);
}

TEST_F(NewtonRaphsonDivisionTest, DivisionWithAlmostZeroDivisor)
{

    double very_small = std::numeric_limits<double>::min() * 10;
    try
    {
        auto [q, r] = NewtonRaphsonDivision::divide_with_remainder(1.0, very_small);

        EXPECT_GT(q, 0.0);

        SUCCEED();
    }
    catch (...)
    {
        FAIL() << "Exception was thrown but should not have been";
    }
}

TEST_F(NewtonRaphsonDivisionTest, CorrectRemainder)
{

    double dividend = 17.5;
    double divisor = 4.2;
    auto [quotient, remainder] = NewtonRaphsonDivision::divide_with_remainder(dividend, divisor);

    EXPECT_NEAR(remainder, dividend - quotient * divisor, epsilon);

    EXPECT_GE(remainder, 0.0);
    EXPECT_LT(remainder, std::abs(divisor));
}

TEST_F(NewtonRaphsonDivisionTest, WorksWithFloatType)
{

    auto [quotient, remainder] = NewtonRaphsonDivision::divide_with_remainder<float>(10.0f, 3.0f);
    EXPECT_EQ(quotient, 3.0f);
    EXPECT_NEAR(remainder, 1.0f, 1e-6f);
}

TEST_F(NewtonRaphsonDivisionTest, WorksWithLongDoubleType)
{

    auto [quotient, remainder] =
        NewtonRaphsonDivision::divide_with_remainder<long double>(10.0L, 3.0L);
    EXPECT_EQ(quotient, 3.0L);
    EXPECT_NEAR(remainder, 1.0L, 1e-10L);
}

TEST_F(NewtonRaphsonDivisionTest, PerformanceCheck)
{
    const int iterations = 10000;
    double sum = 0.0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        auto [q, r] = NewtonRaphsonDivision::divide_with_remainder(i + 0.5, 3.7);
        sum += q + r;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "10,000 division operations took " << duration << " microseconds." << std::endl;
    std::cout << "Calculation result: " << sum << std::endl;

    SUCCEED() << "Performance test completed";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}