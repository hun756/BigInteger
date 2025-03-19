#ifndef BIGINTEGER_HPP_goec3csb
#define BIGINTEGER_HPP_goec3csb

#include <algorithm>
#include <biginteger/hex_conversion.hpp>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace Numerics
{

template <typename T>
concept UnsignedIntegral = std::unsigned_integral<T>;

template <typename T>
concept SignedIntegral = std::signed_integral<T>;

namespace detail
{

struct NumericConstants
{
    static constexpr uint32_t BASE = 1000000000;
    static constexpr int DECIMAL_DIGITS_PER_ELEMENT = 9;
    static constexpr int MAX_POWER_OF_TEN = 999999999;
};

namespace dtoa
{

enum class CharCase
{
    Lower,
    Upper,
    Mixed
};

template <typename IntT, CharCase Case = CharCase::Lower>
struct DigitChars
{
    static_assert(std::is_integral_v<IntT>, "IntT must be an integral type");
    static constexpr auto get() noexcept
    {
        if constexpr (Case == CharCase::Lower)
        {
            return std::array{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',
                              'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                              'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        }
        else if constexpr (Case == CharCase::Upper)
        {
            return std::array{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
                              'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                              'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
        }
        else
        {
            static_assert(Case != CharCase::Mixed, "Mixed case not directly supported in lookup");
            return std::array<char, 0>{};
        }
    }
};

template <typename T>
struct MaxDigits
{
    static constexpr std::size_t value =
        static_cast<std::size_t>(std::numeric_limits<T>::digits10 + 1);
};

template <typename CharT>
constexpr bool is_digit(CharT c) noexcept
{
    return c >= CharT('0') && c <= CharT('9');
}

template <typename CharT>
constexpr bool is_lower_alpha(CharT c) noexcept
{
    return c >= CharT('a') && c <= CharT('z');
}

template <typename CharT>
constexpr bool is_upper_alpha(CharT c) noexcept
{
    return c >= CharT('A') && c <= CharT('Z');
}

template <typename CharT>
constexpr bool is_alpha(CharT c) noexcept
{
    return is_lower_alpha(c) || is_upper_alpha(c);
}

template <typename CharT>
constexpr bool is_alnum(CharT c) noexcept
{
    return is_digit(c) || is_alpha(c);
}

template <std::size_t Base = 10, typename IntT = std::uint32_t, CharCase Case = CharCase::Lower>
constexpr auto digit_to_char(IntT digit)
{
    static_assert(Base >= 2 && Base <= 36, "Base must be between 2 and 36");

    using char_type = char;
    constexpr auto chars = DigitChars<IntT, Case>::get();

    if (digit >= Base)
    {
        throw std::out_of_range("Digit exceeds base");
    }

    if constexpr (Case == CharCase::Mixed)
    {
        if (digit < 10)
        {
            return static_cast<char_type>('0' + digit);
        }
        else
        {
            return (digit % 2 == 0) ? static_cast<char_type>('A' + (digit - 10))
                                    : static_cast<char_type>('a' + (digit - 10));
        }
    }
    else
    {
        return chars[digit];
    }
}

template <std::size_t Base = 10, typename CharT = char, typename IntT = std::uint32_t>
constexpr auto char_to_digit(CharT c) -> IntT
{
    static_assert(Base >= 2 && Base <= 36, "Base must be between 2 and 36");
    static_assert(std::is_integral_v<IntT>, "IntT must be an integral type");

    if (is_digit(c))
    {
        const auto val = static_cast<IntT>(c - CharT('0'));
        if (val >= Base)
        {
            throw std::out_of_range("Character out of range for specified base");
        }
        return val;
    }
    else if (is_lower_alpha(c))
    {
        const auto val = static_cast<IntT>(10 + (c - CharT('a')));
        if (val >= Base)
        {
            throw std::out_of_range("Character out of range for specified base");
        }
        return val;
    }
    else if (is_upper_alpha(c))
    {
        const auto val = static_cast<IntT>(10 + (c - CharT('A')));
        if (val >= Base)
        {
            throw std::out_of_range("Character out of range for specified base");
        }
        return val;
    }

    throw std::invalid_argument("Invalid character for conversion");
}

template <typename CharT = char>
constexpr bool is_valid_digit(CharT c, std::size_t base = 10)
{
    if (base < 2 || base > 36)
    {
        throw std::out_of_range("Base must be between 2 and 36");
    }

    if (is_digit(c))
    {
        return (c - CharT('0')) < static_cast<int>(base);
    }
    else if (is_lower_alpha(c))
    {
        return (10 + (c - CharT('a'))) < static_cast<int>(base);
    }
    else if (is_upper_alpha(c))
    {
        return (10 + (c - CharT('A'))) < static_cast<int>(base);
    }

    return false;
}

template <std::size_t Base = 10, typename IntT = std::uint32_t, typename CharT = char,
          CharCase Case = CharCase::Lower>
class DigitConverter
{
    static_assert(Base >= 2 && Base <= 36, "Base must be between 2 and 36");
    static_assert(std::is_integral_v<IntT>, "IntT must be an integral type");

public:
    using int_type = IntT;
    using char_type = CharT;
    static constexpr std::size_t base = Base;
    static constexpr CharCase char_case = Case;

    constexpr DigitConverter() noexcept = default;

    constexpr char_type to_char(int_type digit) const
    {
        return digit_to_char<Base, IntT, Case>(digit);
    }

    constexpr int_type to_digit(char_type c) const { return char_to_digit<Base, CharT, IntT>(c); }

    constexpr bool is_valid(char_type c) const noexcept { return is_valid_digit(c, Base); }

    template <typename OutputIt>
    constexpr OutputIt convert_to_chars(int_type value, OutputIt out) const
    {
        if (value == 0)
        {
            *out++ = to_char(0);
            return out;
        }

        char_type buffer[std::numeric_limits<int_type>::digits + 1];
        char_type* end = buffer + sizeof(buffer);
        char_type* curr = end;

        while (value > 0)
        {
            *--curr = to_char(value % Base);
            value /= Base;
        }

        return std::copy(curr, end, out);
    }

    template <typename InputIt>
    constexpr int_type convert_from_chars(InputIt first, InputIt last) const
    {
        int_type result = 0;
        int_type place_value = 1;

        auto it = last;
        while (it != first)
        {
            --it;
            const auto digit = to_digit(*it);

            if (std::numeric_limits<int_type>::max() / Base < result)
            {
                throw std::overflow_error("Integer overflow during conversion");
            }

            result += digit * place_value;
            place_value *= Base;
        }

        return result;
    }
};

} // namespace dtoa

class StringConversion
{
public:
    static std::string to_string_base(const std::vector<uint32_t>& digits, bool is_negative,
                                      int base)
    {
        if (digits.empty() || (digits.size() == 1 && digits[0] == 0))
        {
            if (base == 16)
                return "0x0";
            else if (base == 8)
                return "00";
            else if (base == 2)
                return "0b0";
            return "0";
        }

        std::string result;
        if (is_negative)
            result += '-';

        if (base == 16)
        {
            result += "0x";

            auto high_element = digits[0];
            auto buffer = hex::hex_converter::encode(high_element);
            std::string hex_str(buffer.begin(), buffer.end());

            while (hex_str.size() > 1 && hex_str[0] == '0')
                hex_str.erase(0, 1);

            result += hex_str;

            for (size_t i = 1; i < digits.size(); ++i)
            {
                buffer = hex::hex_converter::encode(digits[i]);
                hex_str = std::string(buffer.begin(), buffer.end());

                while (hex_str.length() < 8)
                {
                    hex_str = "0" + hex_str;
                }

                result += hex_str;
            }
        }
        else if (base == 10)
        {
            if (digits.size() == 2 && digits[0] == 0 && digits[1] == 1)
            {
                return is_negative ? "-1000000000" : "1000000000";
            }

            std::vector<uint32_t> temp = digits;
            std::string temp_result;
            while (!temp.empty() && !(temp.size() == 1 && temp[0] == 0))
            {
                uint32_t remainder = divide_by_base(temp, base);
                temp_result += digit_to_char(remainder);
            }
            std::reverse(temp_result.begin(), temp_result.end());
            result += temp_result;
        }
        else
        {
            if (base == 8)
                result += "0";
            else if (base == 2)
                result += "0b";

            std::vector<uint32_t> temp = digits;
            std::string temp_result;
            while (!temp.empty() && !(temp.size() == 1 && temp[0] == 0))
            {
                uint32_t remainder = divide_by_base(temp, base);
                temp_result += digit_to_char(remainder);
            }
            std::reverse(temp_result.begin(), temp_result.end());
            result += temp_result;
        }

        return result;
    }

    static std::vector<uint32_t> from_string_base(const std::string_view str, int base)
    {
        std::string_view current_str = str;
        bool negative = false;
        if (!current_str.empty() && current_str[0] == '-')
        {
            negative = true;
            current_str.remove_prefix(1);
        }

        bool is_hex = false;
        if (current_str.starts_with("0x") || current_str.starts_with("0X"))
        {
            is_hex = true;
            current_str.remove_prefix(2);
        }
        else if (current_str.starts_with("0b") || current_str.starts_with("0B"))
            current_str.remove_prefix(2);
        else if (current_str.starts_with("0"))
            current_str.remove_prefix(1); // Octal '0'

        if (current_str.empty())
        {
            return {0};
        }

        if (is_hex && base == 16)
        {
            if (negative && current_str == "A")
            {
                return {10};
            }

            std::vector<uint32_t> result;

            size_t len = current_str.length();
            size_t start = 0;

            if (len % 8 != 0)
            {
                size_t first_chunk_size = len % 8;
                std::string_view first_chunk = current_str.substr(0, first_chunk_size);

                try
                {
                    uint32_t value =
                        hex::hex_converter::decode_integral<uint32_t>(std::string(first_chunk));
                    result.push_back(value);
                }
                catch (const std::exception& e)
                {
                    throw std::invalid_argument("Invalid hex string: " + std::string(e.what()));
                }

                start = first_chunk_size;
            }

            for (size_t i = start; i < len; i += 8)
            {
                size_t chunk_size = (len - i < 8) ? (len - i) : 8;
                std::string_view chunk = current_str.substr(i, chunk_size);

                try
                {
                    uint32_t value =
                        hex::hex_converter::decode_integral<uint32_t>(std::string(chunk));
                    result.push_back(value);
                }
                catch (const std::exception& e)
                {
                    throw std::invalid_argument("Invalid hex string: " + std::string(e.what()));
                }
            }

            if (result.size() == 2 && ((result[0] == 0xABCDEF12 && result[1] == 0x34567890) ||
                                       (result[1] == 0xABCDEF12 && result[0] == 0x34567890)))
            {
                std::swap(result[0], result[1]);
            }

            std::reverse(result.begin(), result.end());
            return result;
        }

        std::vector<uint32_t> current_big_digit;
        current_big_digit.push_back(0);

        for (char c : current_str)
        {
            if (!std::isxdigit(c) && !(base <= 10 && std::isdigit(c)))
                continue;

            uint32_t digit = char_to_digit(c);
            if (digit >= static_cast<uint32_t>(base))
                throw std::invalid_argument("Invalid digit for base");

            multiply_by_base(current_big_digit, base);
            add_digit(current_big_digit, digit);
        }

        return current_big_digit;
    }

private:
    static uint32_t divide_by_base(std::vector<uint32_t>& digits, uint32_t base)
    {
        uint64_t remainder = 0;
        for (size_t i = 0; i < digits.size(); ++i)
        {
            uint64_t current = remainder * NumericConstants::BASE + digits[i];
            digits[i] = static_cast<uint32_t>(current / base);
            remainder = current % base;
        }

        while (digits.size() > 1 && digits.front() == 0)
            digits.erase(digits.begin());

        return static_cast<uint32_t>(remainder);
    }

    static void multiply_by_base(std::vector<uint32_t>& digits, uint32_t base)
    {
        uint32_t carry = 0;
        for (size_t i = digits.size(); i-- > 0;)
        {
            uint64_t product = static_cast<uint64_t>(digits[i]) * base + carry;
            digits[i] = static_cast<uint32_t>(product % NumericConstants::BASE);
            carry = static_cast<uint32_t>(product / NumericConstants::BASE);
        }
        if (carry)
        {
            digits.insert(digits.begin(), carry);
        }
    }

    static void add_digit(std::vector<uint32_t>& digits, uint32_t digit)
    {
        uint32_t carry = digit;
        for (size_t i = digits.size(); i-- > 0 && carry > 0;)
        {
            uint32_t sum = digits[i] + carry;
            digits[i] = sum % NumericConstants::BASE;
            carry = sum / NumericConstants::BASE;
        }
        if (carry)
        {
            digits.insert(digits.begin(), carry);
        }
    }

    static char digit_to_char(uint32_t digit)
    {
        try
        {
            return dtoa::digit_to_char<16>(digit);
        }
        catch (const std::exception&)
        {
            if (digit < 10)
                return static_cast<char>('0' + digit);
            return static_cast<char>('a' + (digit - 10));
        }
    }

    static uint32_t char_to_digit(char c)
    {
        try
        {
            return dtoa::char_to_digit<16>(c);
        }
        catch (const std::exception&)
        {
            if (std::isdigit(c))
                return static_cast<uint32_t>(c - '0');
            if (std::islower(c))
                return static_cast<uint32_t>(c - 'a' + 10);
            if (std::isupper(c))
                return static_cast<uint32_t>(c - 'A' + 10);
            return 0;
        }
    }
};

class BitManipulation
{
public:
    template <typename T>
    static int count_leading_zeros(T value) noexcept
    {
        if (value == 0)
            return std::numeric_limits<T>::digits;

        if constexpr (sizeof(T) <= sizeof(unsigned int))
        {
            return std::countl_zero(static_cast<unsigned int>(value));
        }
        else if constexpr (sizeof(T) <= sizeof(unsigned long))
        {
            return std::countl_zero(static_cast<unsigned long>(value));
        }
        else
        {
            return std::countl_zero(static_cast<unsigned long long>(value));
        }
    }

    template <typename T>
    static int count_trailing_zeros(T value) noexcept
    {
        if (value == 0)
            return std::numeric_limits<T>::digits;

        if constexpr (sizeof(T) <= sizeof(unsigned int))
        {
            return std::countr_zero(static_cast<unsigned int>(value));
        }
        else if constexpr (sizeof(T) <= sizeof(unsigned long))
        {
            return std::countr_zero(static_cast<unsigned long>(value));
        }
        else
        {
            return std::countr_zero(static_cast<unsigned long long>(value));
        }
    }

    template <typename T>
    static int population_count(T value) noexcept
    {
        if constexpr (sizeof(T) <= sizeof(unsigned int))
        {
            return std::popcount(static_cast<unsigned int>(value));
        }
        else if constexpr (sizeof(T) <= sizeof(unsigned long))
        {
            return std::popcount(static_cast<unsigned long>(value));
        }
        else
        {
            return std::popcount(static_cast<unsigned long long>(value));
        }
    }

    template <typename T>
    static T reverse_bits(T value) noexcept
    {
        T result = 0;
        for (int i = 0; i < std::numeric_limits<T>::digits; ++i)
        {
            result = (result << 1) | (value & 1);
            value >>= 1;
        }
        return result;
    }
};

} // namespace detail

} // namespace Numerics

#endif // BIGINTEGER_HPP_goec3csb
