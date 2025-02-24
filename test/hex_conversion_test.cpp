#include "biginteger/hex_conversion.hpp"
#include <gtest/gtest.h>

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