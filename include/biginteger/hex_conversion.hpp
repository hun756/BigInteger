#ifndef BIGINTEGER_HEX_CONVERSION_HPP_zer7n0
#define BIGINTEGER_HEX_CONVERSION_HPP_zer7n0

#include <array>
#include <cstdint>
#include <type_traits>

namespace hex
{
namespace detail
{

template <typename T>
concept CharType =
    std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char8_t> ||
    std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;

template <typename T>
concept IntegralType = std::is_integral_v<T> && !CharType<T>;

template <typename T>
concept ByteType = sizeof(T) == 1 && std::is_trivial_v<T> && !CharType<T>;

class hex_tables
{
public:
    static constexpr uint32_t lookup_bits = 8;
    static constexpr size_t table_size = 1ULL << lookup_bits;

    template <CharType CharT>
    static constexpr std::array<CharT, 16> encode_table = {
        CharT('0'), CharT('1'), CharT('2'), CharT('3'), CharT('4'), CharT('5'),
        CharT('6'), CharT('7'), CharT('8'), CharT('9'), CharT('A'), CharT('B'),
        CharT('C'), CharT('D'), CharT('E'), CharT('F')};

private:
    template <CharType CharT>
    static constexpr auto make_decode_table()
    {
        std::array<uint8_t, table_size> table{};

        for (uint32_t i = 0; i < table_size; ++i)
        {
            table[i] = 0xFF;
        }

        for (uint8_t i = 0; i < 10; ++i)
        {
            table[static_cast<uint8_t>(CharT('0') + i)] = i;
        }

        for (uint8_t i = 0; i < 6; ++i)
        {
            table[static_cast<uint8_t>(CharT('A') + i)] = 10 + i;
            table[static_cast<uint8_t>(CharT('a') + i)] = 10 + i;
        }

        return table;
    }

public:
    template <CharType CharT>
    static constexpr auto decode_table = make_decode_table<CharT>();

    template <CharType CharT>
    static constexpr bool is_valid_hex_digit(CharT c) noexcept
    {
        const auto uc = static_cast<std::make_unsigned_t<CharT>>(c);
        return uc < table_size && decode_table<CharT>[uc] != 0xFF;
    }
};

} // namespace detail
} // namespace hex

#endif // BIGINTEGER_HEX_CONVERSION_HPP_zer7n0