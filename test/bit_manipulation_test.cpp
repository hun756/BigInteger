#include <biginteger/biginteger.hpp>
#include <cstdint>
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

TEST(BitManipulationTest, CountTrailingZeros)
{
    using namespace Numerics::detail;

    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(0), 32);

    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(1), 0);
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(2), 1);
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(4), 2);
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(8), 3);
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(0x80000000), 31);

    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint8_t>(1), 0);
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint16_t>(2), 1);
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint64_t>(0x8000000000000000), 63);

    // Binary: ...1 0001 0000 0000
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(0x10100), 8);
    // Binary: ...1111 0000
    EXPECT_EQ(BitManipulation::count_trailing_zeros<uint32_t>(0xF0), 4);
}
