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

// ==================================================================================================
// TEST HEX Buffer
// ==================================================================================================

class HexBufferTest : public ::testing::Test
{
protected:
    hex::detail::hex_buffer<char> buffer;
};

TEST_F(HexBufferTest, InitialState)
{
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 64);
}

TEST_F(HexBufferTest, PushBackBasic)
{
    EXPECT_TRUE(buffer.push_back('A'));
    EXPECT_EQ(buffer.size(), 1);
    EXPECT_FALSE(buffer.empty());
    EXPECT_EQ(buffer.front(), 'A');
    EXPECT_EQ(buffer.back(), 'A');
}

TEST_F(HexBufferTest, PushBackMultiple)
{
    const std::string test = "Hello";
    for (char c : test)
    {
        EXPECT_TRUE(buffer.push_back(c));
    }

    EXPECT_EQ(buffer.size(), test.length());
    EXPECT_EQ(buffer.front(), 'H');
    EXPECT_EQ(buffer.back(), 'o');
}

TEST_F(HexBufferTest, CapacityLimit)
{
    // Fill to capacity
    for (size_t i = 0; i < buffer.capacity(); ++i)
    {
        EXPECT_TRUE(buffer.push_back('X'));
    }

    // Try to exceed capacity
    EXPECT_FALSE(buffer.push_back('Y'));
    EXPECT_EQ(buffer.size(), buffer.capacity());
}

TEST_F(HexBufferTest, Clear)
{
    buffer.push_back('A');
    buffer.push_back('B');

    buffer.clear();
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
}

TEST_F(HexBufferTest, PopBack)
{
    buffer.push_back('A');
    buffer.push_back('B');

    buffer.pop_back();
    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer.back(), 'A');

    buffer.pop_back();
    EXPECT_TRUE(buffer.empty());

    // Pop from empty buffer should not crash
    buffer.pop_back();
    EXPECT_TRUE(buffer.empty());
}

TEST_F(HexBufferTest, Iterator)
{
    const std::string test = "Test";
    for (char c : test)
    {
        buffer.push_back(c);
    }

    // Forward iteration
    std::string result;
    for (auto it = buffer.begin(); it != buffer.end(); ++it)
    {
        result += *it;
    }
    EXPECT_EQ(result, test);

    // Range-based for loop
    result.clear();
    for (char c : buffer)
    {
        result += c;
    }
    EXPECT_EQ(result, test);
}

TEST_F(HexBufferTest, View)
{
    const std::string test = "View";
    for (char c : test)
    {
        buffer.push_back(c);
    }

    auto view = buffer.view();
    EXPECT_EQ(view.size(), test.length());

    std::string result(view.begin(), view.end());
    EXPECT_EQ(result, test);
}

TEST_F(HexBufferTest, ConstView)
{
    const std::string test = "ConstView";
    for (char c : test)
    {
        buffer.push_back(c);
    }

    const auto& const_buffer = buffer;
    auto view = const_buffer.view();
    EXPECT_EQ(view.size(), test.length());

    std::string result(view.begin(), view.end());
    EXPECT_EQ(result, test);
}