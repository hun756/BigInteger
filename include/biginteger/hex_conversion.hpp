#ifndef BIGINTEGER_HEX_CONVERSION_HPP_zer7n0
#define BIGINTEGER_HEX_CONVERSION_HPP_zer7n0

#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <span>
#include <stdexcept>
#include <string_view>
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

template <typename T>
class hex_buffer
{
    static_assert(std::is_trivially_copyable_v<T>,
                  "hex_buffer can only store trivially copyable types");

    // 64-byte cache line alignment
    static constexpr size_t buffer_size = 64;
    alignas(64) std::array<T, buffer_size> data_{};
    size_t size_{0};

public:
    // Type definitions for STL container requirements
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    constexpr hex_buffer() noexcept = default;

    [[nodiscard]] constexpr size_type size() const noexcept { return size_; }

    [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

    [[nodiscard]] constexpr size_type capacity() const noexcept { return buffer_size; }

    [[nodiscard]] constexpr size_type max_size() const noexcept { return buffer_size; }

    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept { return data_[pos]; }

    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept
    {
        return data_[pos];
    }

    [[nodiscard]] constexpr reference front() noexcept { return data_[0]; }

    [[nodiscard]] constexpr const_reference front() const noexcept { return data_[0]; }

    [[nodiscard]] constexpr reference back() noexcept { return data_[size_ - 1]; }

    [[nodiscard]] constexpr const_reference back() const noexcept { return data_[size_ - 1]; }

    [[nodiscard]] constexpr pointer data() noexcept { return data_.data(); }

    [[nodiscard]] constexpr const_pointer data() const noexcept { return data_.data(); }

    constexpr void clear() noexcept { size_ = 0; }

    constexpr bool push_back(const T& value) noexcept
    {
        if (size_ < capacity())
        {
            data_[size_++] = value;
            return true;
        }
        return false;
    }

    constexpr bool push_back(T&& value) noexcept
    {
        if (size_ < capacity())
        {
            data_[size_++] = std::move(value);
            return true;
        }
        return false;
    }

    constexpr void pop_back() noexcept
    {
        if (size_ > 0)
        {
            --size_;
        }
    }

    [[nodiscard]] constexpr iterator begin() noexcept { return data(); }

    [[nodiscard]] constexpr const_iterator begin() const noexcept { return data(); }

    [[nodiscard]] constexpr iterator end() noexcept { return data() + size_; }

    [[nodiscard]] constexpr const_iterator end() const noexcept { return data() + size_; }

    [[nodiscard]] constexpr std::span<const T> view() const noexcept
    {
        return std::span<const T>(data(), size_);
    }

    [[nodiscard]] constexpr std::span<T> view() noexcept { return std::span<T>(data(), size_); }
};

template <typename T>
constexpr auto to_underlying(T value) noexcept
{
    return static_cast<std::underlying_type_t<T>>(value);
}

template <typename T>
[[nodiscard]] constexpr T rotl(T x, int s) noexcept
{
    static_assert(std::is_unsigned_v<T>, "rotl requires unsigned type");
    constexpr auto bits = std::numeric_limits<T>::digits;
    const auto r = s % bits;

    if (r == 0)
    {
        return x;
    }

    return (x << r) | (x >> (bits - r));
}

template <typename T>
[[nodiscard]] constexpr T rotr(T x, int s) noexcept
{
    static_assert(std::is_unsigned_v<T>, "rotr requires unsigned type");
    constexpr auto bits = std::numeric_limits<T>::digits;
    const auto r = s % bits;

    if (r == 0)
    {
        return x;
    }

    return (x >> r) | (x << (bits - r));
}

} // namespace detail

template <detail::CharType CharT = char>
class basic_hex_converter
{
    using tables = detail::hex_tables;

public:
    using char_type = CharT;
    using byte_type = std::byte;

    template <typename T>
    static constexpr bool is_hex_digit(T c) noexcept
    {
        if constexpr (detail::CharType<T>)
        {
            return tables::template is_valid_hex_digit<T>(c);
        }
        else
        {
            return false;
        }
    }

    template <detail::IntegralType T>
    [[nodiscard]] static constexpr detail::hex_buffer<CharT> encode(T value) noexcept
    {
        detail::hex_buffer<CharT> result;
        constexpr size_t digits = sizeof(T) * 2;

        if constexpr (std::endian::native == std::endian::little)
        {
            auto unsigned_value = static_cast<std::make_unsigned_t<T>>(value);
            unsigned_value = detail::rotl(unsigned_value, digits * 4);

            for (size_t i = 0; i < digits; ++i)
            {
                const auto nibble = (unsigned_value >> (i * 4)) & 0xF;
                result.push_back(tables::template encode_table<CharT>[nibble]);
            }
        }
        else
        {
            auto unsigned_value = static_cast<std::make_unsigned_t<T>>(value);

            for (size_t i = 0; i < digits; ++i)
            {
                const auto shift = (digits - 1 - i) * 4;
                const auto nibble = (unsigned_value >> shift) & 0xF;
                result.push_back(tables::template encode_table<CharT>[nibble]);
            }
        }

        return result;
    }

    template <typename InputIt>
    [[nodiscard]] static constexpr detail::hex_buffer<CharT> encode(InputIt first,
                                                                    InputIt last) noexcept
    {
        detail::hex_buffer<CharT> result;
        using value_type = typename std::iterator_traits<InputIt>::value_type;
        static_assert(detail::ByteType<value_type>, "Iterator value type must be a byte type");

        while (first != last && result.size() < result.capacity() - 1)
        {
            const auto byte = static_cast<uint8_t>(*first++);
            result.push_back(tables::template encode_table<CharT>[byte >> 4]);
            result.push_back(tables::template encode_table<CharT>[byte & 0xF]);
        }

        return result;
    }

    template <typename Container>
    [[nodiscard]] static constexpr auto decode(std::basic_string_view<CharT> hex)
    {
        using value_type = typename Container::value_type;
        static_assert(detail::ByteType<value_type>, "Container value type must be a byte type");

        if (hex.size() % 2 != 0)
        {
            throw std::invalid_argument("Invalid hex string length - must be even");
        }

        Container result;
        result.reserve(hex.size() / 2);

        for (size_t i = 0; i < hex.size(); i += 2)
        {
            const auto high = tables::template decode_table<CharT>[static_cast<uint8_t>(hex[i])];
            const auto low = tables::template decode_table<CharT>[static_cast<uint8_t>(hex[i + 1])];

            if (high == 0xFF || low == 0xFF)
            {
                throw std::invalid_argument("Invalid hex character");
            }

            result.push_back(static_cast<value_type>((high << 4) | low));
        }

        return result;
    }

    template <detail::IntegralType T>
    [[nodiscard]] static constexpr T decode_integral(std::basic_string_view<CharT> hex)
    {
        if (hex.size() > sizeof(T) * 2)
        {
            throw std::overflow_error("Hex string too large for target type");
        }

        std::make_unsigned_t<T> result = 0;
        const size_t shift = (sizeof(T) * 2 - hex.size()) * 4;

        for (size_t i = 0; i < hex.size(); ++i)
        {
            const auto value = tables::template decode_table<CharT>[static_cast<uint8_t>(hex[i])];
            if (value == 0xFF)
            {
                throw std::invalid_argument("Invalid hex character");
            }
            result = (result << 4) | value;
        }

        if constexpr (std::endian::native == std::endian::little)
        {
            result = detail::rotr(result, shift);
        }
        else
        {
            result <<= shift;
        }

        return static_cast<T>(result);
    }
};

using hex_converter = basic_hex_converter<char>;
using whex_converter = basic_hex_converter<wchar_t>;
using u8hex_converter = basic_hex_converter<char8_t>;
using u16hex_converter = basic_hex_converter<char16_t>;
using u32hex_converter = basic_hex_converter<char32_t>;

} // namespace hex

#endif // BIGINTEGER_HEX_CONVERSION_HPP_zer7n0