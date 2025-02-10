#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>


TEST(NumericConstantsTest, ConstantsAreCorrect)
{
    using namespace Numerics::detail;

    EXPECT_EQ(NumericConstants::BASE, 1000000000);
    EXPECT_EQ(NumericConstants::DECIMAL_DIGITS_PER_ELEMENT, 9);
    EXPECT_EQ(NumericConstants::MAX_POWER_OF_TEN, 999999999);
}

TEST(StringConversionTest, ToStringBaseDecimal)
{
    using namespace Numerics::detail;
    
    std::vector<uint32_t> digits1 = {123, 456, 789};
    EXPECT_EQ(StringConversion::to_string_base(digits1, false, 10),
              "123456789");

    std::vector<uint32_t> digits2 = {0};
    EXPECT_EQ(StringConversion::to_string_base(digits2, false, 10), "0");

    std::vector<uint32_t> digits3 = {1};
    EXPECT_EQ(StringConversion::to_string_base(digits3, true, 10),
              "-1"); // Negatif test

    std::vector<uint32_t> digits4 = {999999999, 999999999};
    EXPECT_EQ(StringConversion::to_string_base(digits4, false, 10),
              "999999999999999999");
}