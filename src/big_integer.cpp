#include "big_integer.hpp"
#include <sstream>
#include <algorithm>


BigInteger::BigInteger(std::string& str) 
    :   digits({})
{
    if (str[0] == '-')
        sign = false;

    digits.assign(str.begin() + 1, str.end());
}


BigInteger::BigInteger(int val) 
{
    val < 0 ? sign = false : sign = true;
    
    std::stringstream ss;
    ss << val;

    digits.assign(ss.str().begin(), ss.str().end());
}


BigInteger::BigInteger(std::vector<int8_t>& biObj) 
    :   digits(biObj)
{
}


BigInteger::BigInteger(const BigInteger& biObj) 
{

}


BigInteger::BigInteger(const BigInteger&& biObj) 
{

}


BigInteger& BigInteger::operator =(const BigInteger& biObj) 
{

}


BigInteger&& BigInteger::operator =(const BigInteger&& biObj) 
{

}


BigInteger&& BigInteger::operator =(const BigInteger&& biObj) 
{

}


BigInteger operator +(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator -(const BigInteger& lhs, const BigInteger& rhs) 
{

}



BigInteger operator *(const BigInteger& lhs, const BigInteger& rhs) 
{

}

BigInteger operator /(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator %(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator +=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator -=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator *=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator /=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator <(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator >=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator >(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator >=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator ==(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator !=(const BigInteger& lhs, const BigInteger& rhs) 
{

}


BigInteger operator ++(BigInteger& biObj) 
{

}


BigInteger operator ++(BigInteger& biObj, int) 
{

}


BigInteger operator --(BigInteger& biObj) 
{

}


BigInteger operator --(BigInteger& biObj, int) 
{

}


std::string BigInteger::toString() 
{
    std::stringstream ss;

    std::copy(
        digits.begin(), 
        digits.end(), 
        std::ostream_iterator<std::string>(ss,"")
    );

    return ss.str();
}


std::unique_ptr<std::byte[]> BigInteger::toByteArray() 
{

}


BigInteger gcd(BigInteger& lhs, BigInteger& rhs) 
{

}


BigInteger pow(BigInteger& biObjs, size_t power) 
{

}


BigInteger abs(BigInteger& biObjs) 
{

}