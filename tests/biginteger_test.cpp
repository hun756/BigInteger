#include <gtest/gtest.h>
#include <biginteger/biginteger.hpp>

TEST(BigIntegerTest, Constructor) {
    BigInteger a("123");
    EXPECT_EQ(a.toString(), "123");
    
    BigInteger b(456);
    EXPECT_EQ(b.toString(), "456");
}

TEST(BigIntegerTest, Addition) {
    BigInteger a("123");
    BigInteger b("456");
    EXPECT_EQ((a + b).toString(), "579");
}
