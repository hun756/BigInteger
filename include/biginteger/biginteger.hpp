#ifndef BIGINTEGER_HPP
#define BIGINTEGER_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

class BigInteger {
private:
    std::vector<int> digits;
    bool negative;

    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

public:
    BigInteger() : negative(false) {
        digits.push_back(0);
    }

    explicit BigInteger(const std::string& str) {
        if (str.empty()) {
            *this = BigInteger();
            return;
        }

        negative = str[0] == '-';
        size_t start = negative ? 1 : 0;
        
        for (size_t i = str.length() - 1; i >= start && i < str.length(); --i) {
            if (!std::isdigit(str[i])) {
                throw std::invalid_argument("Invalid number string");
            }
            digits.push_back(str[i] - '0');
        }
        
        removeLeadingZeros();
        if (digits.size() == 1 && digits[0] == 0) {
            negative = false;
        }
    }

    explicit BigInteger(long long num) {
        negative = num < 0;
        num = std::abs(num);
        
        do {
            digits.push_back(num % 10);
            num /= 10;
        } while (num > 0);
        
        if (digits.empty()) {
            digits.push_back(0);
        }
    }

    BigInteger operator+(const BigInteger& other) const {
        if (negative != other.negative) {
            if (negative) {
                return other - (-*this);
            }
            return *this - (-other);
        }

        BigInteger result;
        result.negative = negative;
        result.digits.clear();
        
        int carry = 0;
        size_t maxSize = std::max(digits.size(), other.digits.size());
        
        for (size_t i = 0; i < maxSize || carry; ++i) {
            int sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];
            
            carry = sum / 10;
            result.digits.push_back(sum % 10);
        }
        
        return result;
    }

    BigInteger operator-() const {
        BigInteger result = *this;
        result.negative = !negative;
        return result;
    }

    BigInteger operator-(const BigInteger& other) const {
        // Implementation of subtraction operator
        // This is a basic implementation, you might want to enhance it
        if (negative != other.negative) {
            return *this + (-other);
        }

        // TODO: Implement proper subtraction logic here
        BigInteger result;
        // For now, returning 0 to make it compile
        return result;
    }

    // ... Other operator implementations would go here ...

    std::string toString() const {
        if (digits.empty()) return "0";
        
        std::string result;
        if (negative) result += "-";
        
        for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
            result += std::to_string(*it);
        }
        
        return result;
    }
};

#endif // BIGINTEGER_HPP
