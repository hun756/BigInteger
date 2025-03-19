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

TEST(BitManipulationTest, PopulationCount)
{
    using namespace Numerics::detail;

    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0), 0);

    EXPECT_EQ(BitManipulation::population_count<uint32_t>(1), 1);
    EXPECT_EQ(BitManipulation::population_count<uint32_t>(2), 1);
    EXPECT_EQ(BitManipulation::population_count<uint32_t>(4), 1);
    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0x80000000), 1);

    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0xFF), 8);
    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0xF0F0), 8);
    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0xAAAAAAAA), 16);
    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0x55555555), 16);

    // Binary: 01010101
    EXPECT_EQ(BitManipulation::population_count<uint8_t>(0x55), 4);
    EXPECT_EQ(BitManipulation::population_count<uint16_t>(0xFFFF), 16);
    EXPECT_EQ(BitManipulation::population_count<uint64_t>(0x8000000000000000), 1);

    EXPECT_EQ(BitManipulation::population_count<uint32_t>(0xFFFFFFFF), 32);
}

TEST(BitManipulationTest, ReverseBits)
{
    using namespace Numerics::detail;

    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0), 0);

    // 00000001 -> 10000000
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0x01), 0x80);
    // 10000000 -> 00000001
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0x80), 0x01);

    // 01010101 -> 10101010
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0x55), 0xAA);
    // 10101010 -> 01010101
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0xAA), 0x55);

    // 00010010 -> 01001000
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0x12), 0x48);
    // 11110000 -> 00001111
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0xF0), 0x0F);

    // 11111111 -> 11111111
    EXPECT_EQ(BitManipulation::reverse_bits<uint8_t>(0xFF), 0xFF);

    // 16-bit/32bit reversal
    EXPECT_EQ(BitManipulation::reverse_bits<uint16_t>(0x0001), 0x8000);
    EXPECT_EQ(BitManipulation::reverse_bits<uint32_t>(0x00000001), 0x80000000);

    // 0001001000110100 -> 0010110001001000
    EXPECT_EQ(BitManipulation::reverse_bits<uint16_t>(0x1234), 0x2C48);
    EXPECT_EQ(BitManipulation::reverse_bits<uint32_t>(0x12345678), 0x1E6A2C48);
}
