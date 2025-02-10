#ifndef BIGINTEGER_HPP_goec3csb
#define BIGINTEGER_HPP_goec3csb

#include <concepts>
#include <cstdint>

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

} // namespace detail

} // namespace Numerics

#endif // BIGINTEGER_HPP_goec3csb
