#ifndef BIG_INTEGER_HPP
#define BIG_INTEGER_HPP


#include <vector>
#include <string>
#include <memory>


class BigInteger
{
public:
    ///< -- Life Time Management --

    /**
     * @brief Construct a new Big Integer object
     * 
     */
    BigInteger() = default;


    /**
     * @brief Construct a new Big Integer object
     * 
     * @param str 
     */
    BigInteger(std::string& str);


    /**
     * @brief Construct a new Big Integer object
     * 
     * @param val 
     */
    BigInteger(int val);


    /**
     * @brief Construct a new Big Integer object
     * 
     * @param biObj 
     */
    BigInteger(std::vector<int8_t>& biObj);


    /**
     * @brief Construct a new Big Integer object
     * 
     * @param biObj 
     */
    BigInteger(const BigInteger& biObj);

    
    /**
     * @brief 
     * 
     * @param biObj 
     * @return BigInteger& 
     */
    BigInteger& operator =(const BigInteger& biObj);


    /**
     * @brief Construct a new Big Integer object
     * 
     * @param biObj 
     */
    BigInteger(BigInteger&& biObj);

    
    /**
     * @brief 
     * 
     * @param biObj 
     * @return BigInteger&& 
     */
    BigInteger&& operator =(BigInteger&& biObj);

    ///< -- Life Time Management End --


    ///< --  Operator Overlodings --

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator +(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator -(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator *(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator /(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator %(const BigInteger& lhs, const BigInteger& rhs);


    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator +=(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator -=(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator *=(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return BigInteger 
     */
    friend BigInteger operator /=(const BigInteger& lhs, const BigInteger& rhs);
    

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return true 
     * @return false 
     */
    friend bool operator <(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return true 
     * @return false 
     */
    friend bool operator <=(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return true 
     * @return false 
     */
    friend bool operator >(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return true 
     * @return false 
     */
    friend bool operator >=(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return true 
     * @return false 
     */
    friend bool operator ==(const BigInteger& lhs, const BigInteger& rhs);

    /**
     * @brief 
     * 
     * @param lhs 
     * @param rhs 
     * @return true 
     * @return false 
     */
    friend bool operator !=(const BigInteger& lhs, const BigInteger& rhs);


    /**
     * @brief 
     * 
     * @param biObj 
     * @return BigInteger 
     */
    friend BigInteger operator ++(BigInteger& biObj);    

    /**
     * @brief 
     * 
     * @param biObj 
     * @return BigInteger 
     */
    friend BigInteger operator ++(BigInteger& biObj, int);    

    /**
     * @brief 
     * 
     * @param biObj 
     * @return BigInteger 
     */
    friend BigInteger operator --(BigInteger& biObj);

    /**
     * @brief 
     * 
     * @param biObj 
     * @return BigInteger 
     */
    friend BigInteger operator --(BigInteger& biObj, int);    

    ///< --  Operator Overlodings End --


    ///< -- Other Methods --

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string toString();


    /**
     * @brief 
     * 
     * @return std::unique_ptr<std::byte[]> 
     */
    std::unique_ptr<std::byte[]> toByteArray();


    /**
     * @brief Get the Sign object
     * 
     * @return true 
     * @return false 
     */
    bool getSign() const;


    /**
     * @brief Set the Sign object
     * 
     * @param signVal 
     */
    void setSign(bool signVal);

    ///< -- Other Methods End --

private:
    bool sign;
    std::vector<int8_t> digits;
};


namespace BigInt
{
    namespace Helper
    {
        /**
         * @brief 
         * 
         * @param lhs 
         * @param rhs 
         * @return BigInteger 
         */
        BigInteger gcd(BigInteger& lhs, BigInteger& rhs);


        /**
         * @brief 
         * 
         * @param biObjs 
         * @param power 
         * @return BigInteger 
         */
        BigInteger pow(BigInteger& biObjs, size_t power);


        /**
         * @brief 
         * 
         * @param biObjs 
         * @return BigInteger 
         */
        BigInteger abs(BigInteger& biObjs);
    }
}

#endif /* end of include guard :  BIG_INTEGER_HPP */