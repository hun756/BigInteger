#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>
#include <tuple>

TEST(ArithmeticOperationsTest, ExtendedGCD)
{
    using namespace Numerics::detail;

    auto [gcd1, x1, y1] = ArithmeticOperations::extended_gcd<int>(35, 8);
    EXPECT_EQ(gcd1, 1);
    EXPECT_EQ(35 * x1 + 8 * y1, gcd1);

    auto [gcd2, x2, y2] = ArithmeticOperations::extended_gcd<int>(12, 18);
    EXPECT_EQ(gcd2, 6);
    EXPECT_EQ(12 * x2 + 18 * y2, gcd2);

    auto [gcd3, x3, y3] = ArithmeticOperations::extended_gcd<int>(20, 5);
    EXPECT_EQ(gcd3, 5);
    EXPECT_EQ(20 * x3 + 5 * y3, gcd3);

    auto [gcd4, x4, y4] = ArithmeticOperations::extended_gcd<int>(240, 46);
    EXPECT_EQ(gcd4, 2);
    EXPECT_EQ(240 * x4 + 46 * y4, gcd4);

    auto [gcd5, x5, y5] = ArithmeticOperations::extended_gcd<int>(-15, 25);
    EXPECT_EQ(gcd5, -5);
    EXPECT_EQ(-15 * x5 + 25 * y5, gcd5);

    EXPECT_EQ(std::abs(gcd5), 5);

    auto [gcd6, x6, y6] = ArithmeticOperations::extended_gcd<int>(17, 0);
    EXPECT_EQ(gcd6, 17);
    EXPECT_EQ(x6, 1);
    EXPECT_EQ(y6, 0);

    auto [gcd7, x7, y7] = ArithmeticOperations::extended_gcd<int64_t>(123456789, 987654321);
    EXPECT_EQ(gcd7, 9);
    EXPECT_EQ(123456789 * x7 + 987654321 * y7, gcd7);
}

TEST(ArithmeticOperationsTest, ModularMultiply)
{
    using namespace Numerics::detail;

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(3, 5, 13), 15 % 13);
    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(7, 8, 13), 56 % 13);

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(0, 10, 13), 0);
    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(10, 0, 13), 0);

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(12, 5, 13), (12 * 5) % 13);

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(15, 16, 13), (15 % 13 * 16 % 13) % 13);

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int64_t>(123456789LL, 987654321LL, 13LL), 4LL);

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(123, 456, 997), (123 * 456) % 997);

    EXPECT_EQ(ArithmeticOperations::modular_multiply<int>(123, 456, 100000), (123 * 456) % 100000);
}

TEST(ArithmeticOperationsTest, ModularInverse)
{
    using namespace Numerics::detail;

    EXPECT_EQ(ArithmeticOperations::modular_inverse<int>(3, 11), 4);
    EXPECT_EQ(ArithmeticOperations::modular_inverse<int>(7, 15), 13);

    EXPECT_EQ((3 * ArithmeticOperations::modular_inverse<int>(3, 11)) % 11, 1);
    EXPECT_EQ((7 * ArithmeticOperations::modular_inverse<int>(7, 15)) % 15, 1);

    EXPECT_EQ(ArithmeticOperations::modular_inverse<int>(4, 8), 0);
    EXPECT_EQ(ArithmeticOperations::modular_inverse<int>(6, 9), 0);

    EXPECT_EQ(ArithmeticOperations::modular_inverse<int>(1, 5), 1);

    int inv_neg = ArithmeticOperations::modular_inverse<int>(-5, 11);

    EXPECT_EQ(((-5 * inv_neg) % 11 + 11) % 11, 1);

    int inv_large = ArithmeticOperations::modular_inverse<int>(101, 997);
    EXPECT_EQ((101 * inv_large) % 997, 1);

    int64_t large_num = 123456789;
    int64_t large_mod = 1000000007;
    int64_t large_inv = ArithmeticOperations::modular_inverse<int64_t>(large_num, large_mod);
    EXPECT_EQ((large_num * large_inv) % large_mod, 1);

    int a = 17, m = 101;
    int inv_a = ArithmeticOperations::modular_inverse<int>(a, m);
    EXPECT_NE(inv_a, 0);
    EXPECT_EQ((a * inv_a) % m, 1);
}

TEST(ArithmeticOperationsTest, IsPrimeMillerRabin)
{
    using namespace Numerics::detail;

    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(0));
    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(1));
    EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin(2));
    EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin(3));
    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(4));

    std::vector<int> small_primes = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    for (int prime : small_primes)
    {
        EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin(prime))
            << "Failed on prime: " << prime;
    }

    std::vector<int> small_composites = {6,  8,  9,  10, 12, 14, 15, 16, 18,
                                         20, 21, 22, 24, 25, 26, 27, 28};
    for (int composite : small_composites)
    {
        EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(composite))
            << "Failed on composite: " << composite;
    }

    std::vector<int64_t> larger_primes = {997,   1009,  1013,   1019,   10007,
                                          10009, 10037, 100003, 100019, 100043};
    for (int64_t prime : larger_primes)
    {
        EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin(prime))
            << "Failed on prime: " << prime;
    }

    std::vector<int64_t> larger_composites = {999,   1001,   1003,   10001, 10003,
                                              10005, 100001, 100009, 100015};
    for (int64_t composite : larger_composites)
    {
        EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(composite))
            << "Failed on composite: " << composite;
    }

    EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin<int64_t>(2147483647));
    EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin<int64_t>(67280421310721));

    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(-1));
    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(-2));

    EXPECT_TRUE(ArithmeticOperations::is_prime_miller_rabin(1000000007, 50));

    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(561));
    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(1105));
    EXPECT_FALSE(ArithmeticOperations::is_prime_miller_rabin(1729));
}

TEST(ArithmeticOperationsTest, ModularPow)
{
    using namespace Numerics::detail;

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(2, 3, 5), 8 % 5);
    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(3, 4, 7), 81 % 7);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(5, 0, 13), 1);
    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(0, 0, 13), 1);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(0, 5, 13), 0);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(7, 1, 13), 7);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(12, 2, 13), 144 % 13);
    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(12, 3, 13), (12 * 12 * 12) % 13);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(7, 20, 13), 3);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int64_t>(123456789LL, 2LL, 13LL), 1LL);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(2, 16, 17), 1);

    EXPECT_EQ(ArithmeticOperations::modular_pow<int>(2, 100, 101), 1);

    for (int base = 2; base <= 5; base++)
    {
        for (int exp = 2; exp <= 5; exp++)
        {
            int mod = 97;
            int expected = static_cast<int>(std::fmod(std::pow(base, exp), mod));
            EXPECT_EQ(ArithmeticOperations::modular_pow<int>(base, exp, mod), expected);
        }
    }
}
