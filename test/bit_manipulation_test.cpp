#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>

TEST(BitManipulationTest, CountLeadingZeros)
{
    using namespace Numerics::detail;
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint32_t>(0), 32);

    EXPECT_EQ(BitManipulation::count_leading_zeros<uint32_t>(1), 31);
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint32_t>(2), 30);
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint32_t>(4), 29);
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint32_t>(0x80000000), 0);

    // 1 as uint8_t, cast to uint32_t
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint8_t>(1), 31);
    // 1 as uint16_t, cast to uint32_t
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint16_t>(1), 31);
    EXPECT_EQ(BitManipulation::count_leading_zeros<uint64_t>(1), 63);

    EXPECT_EQ(BitManipulation::count_leading_zeros<uint32_t>(0x10000), 15);
}
