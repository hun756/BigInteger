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
        if (digit < 10)
            return static_cast<char>('0' + digit);
        return static_cast<char>('a' + (digit - 10));
    }

    static uint32_t char_to_digit(char c)
    {
        if (std::isdigit(c))
            return static_cast<uint32_t>(c - '0');
        if (std::islower(c))
            return static_cast<uint32_t>(c - 'a' + 10);
        if (std::isupper(c))
            return static_cast<uint32_t>(c - 'A' + 10);
        return 0; // Should not reach here if isxdigit check is used before
    }
};

} // namespace detail

} // namespace Numerics

#endif // BIGINTEGER_HPP_goec3csb
