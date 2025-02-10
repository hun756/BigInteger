#include <BigInteger/BigInteger.hpp>
#include <iostream>

int main()
{
    BigInteger a("123456789012345678901234567890");
    BigInteger b("987654321098765432109876543210");

    std::cout << "A = " << a.toString() << std::endl;
    std::cout << "B = " << b.toString() << std::endl;
    std::cout << "A + B = " << (a + b).toString() << std::endl;

    return 0;
}
