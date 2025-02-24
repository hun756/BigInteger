#include "biginteger/hex_conversion.hpp"
#include <gtest/gtest.h>

// ==================================================================================================
// TEST HEX CONVERSION CONCEPTS
// ==================================================================================================

TEST(HexConceptsTest, CharTypeTest)
{
    static_assert(hex::detail::CharType<char>);
    static_assert(hex::detail::CharType<wchar_t>);
    static_assert(hex::detail::CharType<char8_t>);
    static_assert(hex::detail::CharType<char16_t>);
    static_assert(hex::detail::CharType<char32_t>);

    static_assert(!hex::detail::CharType<int>);
    static_assert(!hex::detail::CharType<unsigned char>);
    static_assert(!hex::detail::CharType<signed char>);
}

TEST(HexConceptsTest, IntegralTypeTest)
{
    static_assert(hex::detail::IntegralType<int>);
    static_assert(hex::detail::IntegralType<unsigned int>);
    static_assert(hex::detail::IntegralType<long>);
    static_assert(hex::detail::IntegralType<unsigned long>);

    static_assert(!hex::detail::IntegralType<char>);
    static_assert(!hex::detail::IntegralType<float>);
    static_assert(!hex::detail::IntegralType<double>);
}

TEST(HexConceptsTest, ByteTypeTest)
{
    static_assert(hex::detail::ByteType<std::byte>);
    static_assert(hex::detail::ByteType<unsigned char>);
    static_assert(hex::detail::ByteType<signed char>);

    static_assert(!hex::detail::ByteType<int>);
    static_assert(!hex::detail::ByteType<char>);
    static_assert(!hex::detail::ByteType<short>);
}

// ==================================================================================================
// TEST HEX TABLES
// ==================================================================================================

class HexTablesTest : public ::testing::Test
{
protected:
    static constexpr auto& encode_table = hex::detail::hex_tables::encode_table<char>;
    static constexpr auto& decode_table = hex::detail::hex_tables::decode_table<char>;
};

TEST_F(HexTablesTest, EncodeTableTest)
{
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(encode_table[i], static_cast<char>('0' + i));
    }

    for (int i = 0; i < 6; ++i)
    {
        EXPECT_EQ(encode_table[10 + i], static_cast<char>('A' + i));
    }
}

TEST_F(HexTablesTest, DecodeTableTest)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(decode_table[static_cast<uint8_t>(c)], c - '0');
    }

    for (char c = 'A'; c <= 'F'; ++c)
    {
        EXPECT_EQ(decode_table[static_cast<uint8_t>(c)], 10 + (c - 'A'));
    }

    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_EQ(decode_table[static_cast<uint8_t>(c)], 10 + (c - 'a'));
    }

    EXPECT_EQ(decode_table[static_cast<uint8_t>('G')], 0xFF);
    EXPECT_EQ(decode_table[static_cast<uint8_t>('g')], 0xFF);
    EXPECT_EQ(decode_table[static_cast<uint8_t>('x')], 0xFF);
    EXPECT_EQ(decode_table[static_cast<uint8_t>(' ')], 0xFF);
}

TEST_F(HexTablesTest, IsValidHexDigitTest)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(c));
    }
    for (char c = 'A'; c <= 'F'; ++c)
    {
        EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(c));
    }
    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(c));
    }

    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit('G'));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit('g'));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit('x'));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit(' '));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit('-'));
}

TEST(HexTablesMultiCharTest, DifferentCharTypes)
{
    EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(L'A'));
    EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(L'f'));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit(L'G'));

    EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(u'5'));
    EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(u'E'));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit(u'x'));

    EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(U'7'));
    EXPECT_TRUE(hex::detail::hex_tables::is_valid_hex_digit(U'c'));
    EXPECT_FALSE(hex::detail::hex_tables::is_valid_hex_digit(U'Z'));
}
