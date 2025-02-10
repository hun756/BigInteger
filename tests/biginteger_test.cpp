#include <gtest/gtest.h>
#include <biginteger/biginteger.hpp>

TEST(NumericConstantsTest, ConstantsAreCorrect) {
    using namespace Numerics::detail;

    EXPECT_EQ(NumericConstants::BASE, 1000000000);
    EXPECT_EQ(NumericConstants::DECIMAL_DIGITS_PER_ELEMENT, 9);
    EXPECT_EQ(NumericConstants::MAX_POWER_OF_TEN, 999999999);
}
