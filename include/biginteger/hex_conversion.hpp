#ifndef BIGINTEGER_HEX_CONVERSION_HPP_zer7n0
#define BIGINTEGER_HEX_CONVERSION_HPP_zer7n0

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

} // namespace detail
} // namespace hex

#endif // BIGINTEGER_HEX_CONVERSION_HPP_zer7n0