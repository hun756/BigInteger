#ifndef BIGINTEGER_HPP_goec3csb
#define BIGINTEGER_HPP_goec3csb

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
    static std::string to_string_base(const std::vector<uint32_t>& digits,
                                      bool is_negative, int base)
    {
        if (digits.empty() || (digits.size() == 1 && digits[0] == 0))
            return "0";

        std::string result;
        result.reserve(digits.size() * 10);

        if (is_negative)
            result += '-';

        std::vector<uint32_t> temp = digits;
        std::string temp_result;
        while (!temp.empty() && !(temp.size() == 1 && temp[0] == 0))
        {
            uint32_t remainder = divide_by_base(temp, base);
            temp_result += digit_to_char(remainder);
        }
        std::reverse(temp_result.begin(), temp_result.end());

        result += temp_result;

        if (base == 16)
            result = "0x" + result;
        else if (base == 8)
            result = "0" + result;
        else if (base == 2)
            result = "0b" + result;

        return result;
    }

    static std::vector<uint32_t> from_string_base(const std::string_view str,
                                                  int base)
    {
        std::string_view current_str = str;
        bool negative = false;
        if (!current_str.empty() && current_str[0] == '-')
        {
            negative = true;
            current_str.remove_prefix(1);
        }

        if (current_str.starts_with("0x") || current_str.starts_with("0X"))
            current_str.remove_prefix(2);
        else if (current_str.starts_with("0b") || current_str.starts_with("0B"))
            current_str.remove_prefix(2);
        else if (current_str.starts_with("0"))
            current_str.remove_prefix(1); // Octal '0'

        std::vector<uint32_t> result;
        result.reserve(current_str.length() / 9 + 1);

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

        while (!digits.empty() && digits.front() == 0 &&
               digits.size() > 1) // keep at least one zero if the number is 0
            digits.erase(digits.begin());

        return static_cast<uint32_t>(remainder %
                                     0xFFFFFFFF); // ensure uint32_t range
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
