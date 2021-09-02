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


BigInteger::BigInteger(BigInteger&& biObj) 
{

}


BigInteger& BigInteger::operator =(const BigInteger& biObj) 
{

}


BigInteger&& BigInteger::operator =( BigInteger&& biObj) 
{

}


BigInteger operator +(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
            return lhs - rhs;

        BigInteger res;

        for (int i = 0, carry = 0; i < lhs.digits.size() || i < rhs.digits.size() || carry; i++) {
            carry += (i < lhs.digits.size() ? lhs.digits[i] - '0' : 0) + (i < rhs.digits.size() ? rhs.digits[i] - '0' : 0);

            res.digits.push_back(carry % 10 + '0');
            carry /= 10;
        }

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


bool operator <(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
        return false;

    return lhs.digits < rhs.digits;
}


bool operator <=(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
        return false;

    return lhs.digits <= rhs.digits;
}


bool operator >(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
        return false;

    return lhs.digits > rhs.digits;
}


bool operator >=(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
        return false;

    return lhs.digits >= rhs.digits;
}


bool operator ==(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
        return false;

    return lhs.digits == rhs.digits;
}


bool operator !=(const BigInteger& lhs, const BigInteger& rhs) 
{
    if (lhs.sign != rhs.sign)
        return false;

    return lhs.digits != rhs.digits;
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
    std::unique_ptr<std::byte[]> newMemBIObj(new std::byte[digits.size()]);

    for (size_t i = 0; i < digits.size(); i++)
    {
        newMemBIObj[i] = static_cast<std::byte>(digits[i]);
    }
    
    return newMemBIObj;
}


bool BigInteger::getSign() const
{
    return sign;
}


void BigInteger::setSign(bool signVal)
{
    this->sign = signVal;
}



BigInteger gcd(BigInteger& lhs, BigInteger& rhs) 
{

}


BigInteger pow(BigInteger& biObjs, size_t power) 
{

}


BigInteger abs(BigInteger& biObjs) 
{
    if(!biObjs.getSign())
    {
        biObjs.setSign(true);
    }

    return biObjs;
}