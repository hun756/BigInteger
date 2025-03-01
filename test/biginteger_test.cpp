#include <algorithm>
#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

TEST(NumericConstantsTest, ConstantsAreCorrect)
{
    using namespace Numerics::detail;

    EXPECT_EQ(NumericConstants::BASE, 1000000000);
    EXPECT_EQ(NumericConstants::DECIMAL_DIGITS_PER_ELEMENT, 9);
    EXPECT_EQ(NumericConstants::MAX_POWER_OF_TEN, 999999999);
}

class StringConversionTest : public ::testing::Test
{
protected:
    void SetUp() override {}

    std::vector<uint32_t> generateRandomDigits(size_t size)
    {
        std::vector<uint32_t> result(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, 999999999);

        for (size_t i = 0; i < size; ++i)
        {
            result[i] = dis(gen);
        }

        return result;
    }
};

TEST_F(StringConversionTest, ToStringBaseDecimal)
{
    using namespace Numerics::detail;

    // basic number
    std::vector<uint32_t> digits1 = {123456789};
    EXPECT_EQ(StringConversion::to_string_base(digits1, false, 10), "123456789");

    // zero
    std::vector<uint32_t> digits2 = {0};
    EXPECT_EQ(StringConversion::to_string_base(digits2, false, 10), "0");

    // negative numbers
    std::vector<uint32_t> digits3 = {1};
    EXPECT_EQ(StringConversion::to_string_base(digits3, true, 10), "-1");

    // large numbers spanning multiple base elements
    std::vector<uint32_t> digits4 = {999999999, 999999999};
    EXPECT_EQ(StringConversion::to_string_base(digits4, false, 10), "999999999999999999");

    // numbers with leading zeros in later elements
    std::vector<uint32_t> digits5 = {123, 456};
    EXPECT_EQ(StringConversion::to_string_base(digits5, false, 10), "123000000456");

    // Büyük sayılar için sınır durum
    std::vector<uint32_t> digits6 = {0, 1};
    EXPECT_EQ(StringConversion::to_string_base(digits6, false, 10), "1000000000");

    // Boş vektör
    std::vector<uint32_t> digits7 = {};
    EXPECT_EQ(StringConversion::to_string_base(digits7, false, 10), "0");
}

TEST_F(StringConversionTest, ToStringBaseHexadecimal)
{
    using namespace Numerics::detail;

    // basic case
    std::vector<uint32_t> digits1 = {255, 0};
    EXPECT_EQ(StringConversion::to_string_base(digits1, false, 16), "0xFF00000000");

    // zero
    std::vector<uint32_t> digits2 = {0};
    EXPECT_EQ(StringConversion::to_string_base(digits2, false, 16), "0x0");

    // small number
    std::vector<uint32_t> digits3 = {10};
    EXPECT_EQ(StringConversion::to_string_base(digits3, false, 16), "0xA");

    // max uint32
    std::vector<uint32_t> digits4 = {0xFFFFFFFF};
    EXPECT_EQ(StringConversion::to_string_base(digits4, false, 16), "0xFFFFFFFF");

    // negative number
    std::vector<uint32_t> digits5 = {255};
    EXPECT_EQ(StringConversion::to_string_base(digits5, true, 16), "-0xFF");

    // multiple elements
    std::vector<uint32_t> digits6 = {0xABCD, 0xEF12};
    EXPECT_EQ(StringConversion::to_string_base(digits6, false, 16), "0xABCD0000EF12");

    // Sınır durumları
    std::vector<uint32_t> digits7 = {0, 0, 1};
    EXPECT_EQ(StringConversion::to_string_base(digits7, false, 16), "0x000000000000000001");
}

TEST_F(StringConversionTest, ToStringBaseBinary)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> digits1 = {5};
    EXPECT_EQ(StringConversion::to_string_base(digits1, false, 2), "0b101");

    std::vector<uint32_t> digits2 = {0};
    EXPECT_EQ(StringConversion::to_string_base(digits2, false, 2), "0b0");

    std::vector<uint32_t> digits3 = {7};
    EXPECT_EQ(StringConversion::to_string_base(digits3, true, 2), "-0b111");

    std::vector<uint32_t> digits4 = {170};
    EXPECT_EQ(StringConversion::to_string_base(digits4, false, 2), "0b10101010");
}

TEST_F(StringConversionTest, ToStringBaseOctal)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> digits1 = {8};
    EXPECT_EQ(StringConversion::to_string_base(digits1, false, 8), "010");

    std::vector<uint32_t> digits2 = {0};
    EXPECT_EQ(StringConversion::to_string_base(digits2, false, 8), "00");

    std::vector<uint32_t> digits3 = {15};
    EXPECT_EQ(StringConversion::to_string_base(digits3, true, 8), "-017");

    std::vector<uint32_t> digits4 = {511};
    EXPECT_EQ(StringConversion::to_string_base(digits4, false, 8), "0777");
}

TEST_F(StringConversionTest, FromStringBaseDecimal)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> expected1 = {123456789};
    EXPECT_EQ(StringConversion::from_string_base("123456789", 10), expected1);

    std::vector<uint32_t> expected2 = {0};
    EXPECT_EQ(StringConversion::from_string_base("0", 10), expected2);

    std::vector<uint32_t> expected3 = {1};
    EXPECT_EQ(StringConversion::from_string_base("-1", 10), expected3);

    std::vector<uint32_t> expected4 = {999999999, 999999999};
    EXPECT_EQ(StringConversion::from_string_base("999999999999999999", 10), expected4);

    std::vector<uint32_t> expected5 = {123};
    EXPECT_EQ(StringConversion::from_string_base("123", 10), expected5);
    EXPECT_EQ(StringConversion::from_string_base("+123", 10), expected5);

    EXPECT_THROW({ StringConversion::from_string_base("123A", 10); }, std::invalid_argument);
}

// Yeni test: from_string_base için hexadecimal testi
TEST_F(StringConversionTest, FromStringBaseHexadecimal)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> expected1 = {255};
    EXPECT_EQ(StringConversion::from_string_base("0xFF", 16), expected1);
    EXPECT_EQ(StringConversion::from_string_base("FF", 16), expected1);
    EXPECT_EQ(StringConversion::from_string_base("0xff", 16), expected1);
    EXPECT_EQ(StringConversion::from_string_base("ff", 16), expected1);

    std::vector<uint32_t> expected2 = {0};
    EXPECT_EQ(StringConversion::from_string_base("0x0", 16), expected2);
    EXPECT_EQ(StringConversion::from_string_base("0", 16), expected2);

    std::vector<uint32_t> expected3 = {10};
    EXPECT_EQ(StringConversion::from_string_base("-0xA", 16), expected3);
    EXPECT_EQ(StringConversion::from_string_base("-A", 16), expected3);

    std::vector<uint32_t> expected4 = {0xABCDEF12, 0x34567890};
    EXPECT_EQ(StringConversion::from_string_base("0xABCDEF1234567890", 16), expected4);

    EXPECT_THROW({ StringConversion::from_string_base("0xGHIJ", 16); }, std::invalid_argument);
}

TEST_F(StringConversionTest, FromStringBaseBinary)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> expected1 = {5}; // 101 binary
    EXPECT_EQ(StringConversion::from_string_base("0b101", 2), expected1);
    EXPECT_EQ(StringConversion::from_string_base("101", 2), expected1);

    std::vector<uint32_t> expected2 = {0};
    EXPECT_EQ(StringConversion::from_string_base("0b0", 2), expected2);
    EXPECT_EQ(StringConversion::from_string_base("0", 2), expected2);

    std::vector<uint32_t> expected3 = {170}; // 10101010 binary
    EXPECT_EQ(StringConversion::from_string_base("0b10101010", 2), expected3);

    EXPECT_THROW({ StringConversion::from_string_base("0b1012", 2); }, std::invalid_argument);
}

TEST_F(StringConversionTest, FromStringBaseOctal)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> expected1 = {8};
    EXPECT_EQ(StringConversion::from_string_base("010", 8), expected1);
    EXPECT_EQ(StringConversion::from_string_base("10", 8), expected1);

    std::vector<uint32_t> expected2 = {0};
    EXPECT_EQ(StringConversion::from_string_base("00", 8), expected2);
    EXPECT_EQ(StringConversion::from_string_base("0", 8), expected2);

    std::vector<uint32_t> expected3 = {511};
    EXPECT_EQ(StringConversion::from_string_base("0777", 8), expected3);

    EXPECT_THROW({ StringConversion::from_string_base("0789", 8); }, std::invalid_argument);
}

TEST_F(StringConversionTest, FromStringBaseMixed)
{
    using namespace Numerics::detail;

    std::vector<uint32_t> expected1 = {123};
    EXPECT_EQ(StringConversion::from_string_base("1 2 3", 10), expected1);
    EXPECT_EQ(StringConversion::from_string_base("1,2.3", 10), expected1);

    std::vector<uint32_t> expected2 = {0xABCDEF};
    EXPECT_EQ(StringConversion::from_string_base("0xAbCdEf", 16), expected2);

    std::vector<uint32_t> expected3 = {123};
    EXPECT_EQ(StringConversion::from_string_base("0000123", 10), expected3);

    std::vector<uint32_t> expected4 = {15};
    EXPECT_EQ(StringConversion::from_string_base("15", 10), expected4);
    EXPECT_EQ(StringConversion::from_string_base("F", 16), expected4);
    EXPECT_EQ(StringConversion::from_string_base("17", 8), expected4);
    EXPECT_EQ(StringConversion::from_string_base("1111", 2), expected4);
}

TEST_F(StringConversionTest, DISABLED_PerformanceTest)
{
    using namespace Numerics::detail;

    auto largeNumber = generateRandomDigits(100);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; i++)
    {
        auto str = StringConversion::to_string_base(largeNumber, false, 10);
        auto digits = StringConversion::from_string_base(str, 10);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Performance test completed in " << duration << " ms" << std::endl;

    SUCCEED();
}

// TEST_F(StringConversionTest, EdgeCases)
// {
//     using namespace Numerics::detail;

//     EXPECT_THROW({ StringConversion::to_string_base({1}, false, 1); }, std::invalid_argument);

//     EXPECT_THROW({ StringConversion::to_string_base({1}, false, 37); }, std::invalid_argument);

//     EXPECT_THROW({ StringConversion::from_string_base("123", 1); }, std::invalid_argument);

//     EXPECT_THROW({ StringConversion::from_string_base("123", 37); }, std::invalid_argument);

//     EXPECT_THROW({ StringConversion::from_string_base("", 10); }, std::invalid_argument);

//     EXPECT_THROW({ StringConversion::from_string_base("-", 10); }, std::invalid_argument);

//     std::vector<uint32_t> original = {0x12345678, 0x9ABCDEF0};
//     auto hexString = StringConversion::to_string_base(original, false, 16);
//     auto roundTrip = StringConversion::from_string_base(hexString, 16);
//     EXPECT_EQ(roundTrip, original);
// }
