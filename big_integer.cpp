#include <big_integer.h>
BigInteger::BigInteger() {
    digits_ = {0};
    negative_ = false;
}
BigInteger::BigInteger(int value) {
    if (value < 0) {
        negative_ = true;
        value = -value;
    } else {
        negative_ = false;
    }
    do {
        digits_.push_back(value % 10);
        value /= 10;
    } while (value > 0);
}
BigInteger::BigInteger(long long value) {
    if (value < 0) {
        negative_ = true;
        value = -value;
    } else {
        negative_ = false;
    }
    do {
        digits_.push_back(value % 10);
        value /= 10;
    } while (value > 0);
}
BigInteger::BigInteger(const std::string& str) {
    size_t start = 0;
    if (str[0] == '-') {
        negative_ = true;
        start = 1;
    }
    for (int i = str.length() - 1; i >= (int)start; --i) {
        digits_.push_back(str[i] - '0');
    }
    while (digits_.size() > 1 && digits_.back() == '0') {
        digits_.pop_back();
    }
}
std::string BigInteger::to_string() const {
    std::string result;
    if (negative_) {
        result += '-';
    }
    for (int i = digits_.size() - 1; i >= 0; --i) {
        result += static_cast<char>(digits_[i] + '0');
    }
    return result;
}
bool BigInteger::is_zero() const {
    return digits_.size() == 1 && digits_[0] == 0;
}
bool BigInteger::is_negative() const {
    return negative_;
}
BigInteger::operator bool() const {
    return !is_zero();
}
bool BigInteger::operator<(const BigInteger& rhs) const {
    if (negative_ && !rhs.negative_) {
        return true;
    }
    if (!negative_ && rhs.negative_) {
        return false;
    }
    if (digits_.size() != rhs.digits_.size()) {
        if (!negative_) {
            return digits_.size() < rhs.digits_.size();
        } else {
            return digits_.size() > rhs.digits_.size();
        }
    }
    for (int i = digits_.size() - 1; i > -1; --i) {
        if (digits_[i] < rhs.digits_[i]) return !is_negative();
        if (digits_[i] > rhs.digits_[i]) return is_negative();
    }
    return 0;
}
bool BigInteger::operator>(const BigInteger& rhs) const {
    return rhs < *this;
}
bool BigInteger::operator<=(const BigInteger& rhs) const {
    return !(rhs < *this);
}
bool BigInteger::operator>=(const BigInteger& rhs) const {
    return !(*this < rhs);
}
bool BigInteger::operator==(const BigInteger& rhs) const {
    return negative_ == rhs.negative_ && digits_ == rhs.digits_;
}
bool BigInteger::operator!=(const BigInteger& rhs) const {
    return !(*this == rhs);
}
BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    if (!result.is_zero()) {
        result.negative_ = !result.negative_;
    }
    return result;
}
BigInteger& BigInteger::operator++() {
    *this += 1;
    return *this;
}
BigInteger& BigInteger::operator--() {
    *this -= 1;
    return *this;
}
BigInteger BigInteger::operator++(int) {
    BigInteger old = *this;
    ++(*this);
    return old;
}
BigInteger BigInteger::operator--(int) {
    BigInteger old = *this;
    --(*this);
    return old;
}
int BigInteger::cmp_abs(const BigInteger& other) const {
    if (digits_.size() != other.digits_.size()) {
        return digits_.size() < other.digits_.size() ? -1 : 1;
    }
    for (int i = digits_.size() - 1; i >= 0; --i) {
        if (digits_[i] != other.digits_[i]) {
            return digits_[i] < other.digits_[i] ? -1 : 1;
        }
    }
    return 0;
}
void BigInteger::add_abs(const BigInteger& other) {
    int carry = 0;
    size_t max_size = digits_.size();
    if (other.digits_.size() > max_size) {
        max_size = other.digits_.size();
    }
    while (digits_.size() < max_size + 1) {
        digits_.push_back(0);
    }
    for (size_t i = 0; i < max_size; ++i) {
        int a = digits_[i];
        int b = (i < other.digits_.size()) ? other.digits_[i] : 0;
        int sum = a + b + carry;
        digits_[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry > 0) {
        digits_[max_size] = carry;
    } else {
        while (digits_.size() > 1 && digits_.back() == 0) {
            digits_.pop_back();
        }
    }
}
void BigInteger::sub_abs(const BigInteger& other) {
    int borrow = 0;
    for (size_t i = 0; i < digits_.size(); ++i) {
        int b = (i < other.digits_.size()) ? other.digits_[i] : 0;
        int diff = digits_[i] - b - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        digits_[i] = diff;
    }
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }
}
void BigInteger::mul_abs(const BigInteger& other) {
    std::vector<int> result(digits_.size() + other.digits_.size());
    for (size_t i = 0; i < digits_.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.digits_.size(); ++j) {
            int prod = result[i + j] + digits_[i] * other.digits_[j] + carry;
            result[i + j] = prod % 10; 
            carry = prod / 10;
        }
        if (carry > 0) {
            result[i + other.digits_.size()] += carry;
        }
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    digits_ = result;
}
BigInteger BigInteger::div_mod_abs(const BigInteger& divisor) {
    BigInteger dividend = *this;
    digits_.clear();
    if (dividend.cmp_abs(divisor) < 0) {
        digits_ = {0};
        return dividend; 
    }
    BigInteger remainder;
    remainder.digits_.clear();
    for (int i = dividend.digits_.size() - 1; i >= 0; --i) {
        remainder.digits_.insert(remainder.digits_.begin(), dividend.digits_[i]);
        while (remainder.digits_.size() > 1 && remainder.digits_.back() == 0) {
            remainder.digits_.pop_back();
        }
        int digit = 0;
        while (remainder.cmp_abs(divisor) >= 0) {
            remainder.sub_abs(divisor);
            ++digit;
        }
        digits_.insert(digits_.begin(), digit);
    }
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }
    if (digits_.empty()) {
        digits_ = {0};
    }
    return remainder; 
}
BigInteger& BigInteger::operator+=(const BigInteger& rhs) {
    if (negative_ == rhs.negative_) {
        add_abs(rhs);
        return *this;
    }
    int cmp = cmp_abs(rhs);
    if (cmp == 0) {
        digits_ = {0};
        negative_ = false;
        return *this;
    }
    if (cmp > 0) {
        sub_abs(rhs);
    } else {
        BigInteger temp = rhs;
        temp.negative_ = false;
        temp.sub_abs(*this);
        *this = temp;
        negative_ = !negative_;
    }
    return *this;
}
BigInteger BigInteger::operator+(const BigInteger& rhs) const {
    BigInteger result = *this;
    result += rhs;  
    return result;
}
BigInteger& BigInteger::operator-=(const BigInteger& rhs) {
    *this += (-rhs);  
    return *this;
}
BigInteger BigInteger::operator-(const BigInteger& rhs) const {
    BigInteger result = *this; 
    result -= rhs;   
    return result;
}
BigInteger& BigInteger::operator*=(const BigInteger& rhs) {
    bool result_negative = (negative_ != rhs.negative_);
    mul_abs(rhs);
    negative_ = result_negative;
    if (is_zero()) {
        negative_ = false;
    }
    return *this;
}
BigInteger BigInteger::operator*(const BigInteger& rhs) const {
    BigInteger result = *this;
    result *= rhs;
    return result;
}
BigInteger& BigInteger::operator/=(const BigInteger& rhs) {
    if (rhs.is_zero()) {
        return *this;
    }
    bool result_negative = (negative_ != rhs.negative_);
    BigInteger dividend = *this;
    dividend.negative_ = false; 
    BigInteger divisor = rhs;
    divisor.negative_ = false;
    dividend.div_mod_abs(divisor);
    *this = dividend;
    negative_ = result_negative;
    if (is_zero()) {
        negative_ = false;
    }
    return *this;
}
BigInteger BigInteger::operator/(const BigInteger& rhs) const {
    BigInteger result = *this;
    result /= rhs;
    return result;
}
BigInteger& BigInteger::operator%=(const BigInteger& rhs) {
    BigInteger quotient = *this / rhs;
    *this = *this - quotient * rhs;
    return *this;
}
BigInteger BigInteger::operator%(const BigInteger& rhs) const {
    BigInteger result = *this;
    result %= rhs;
    return result;
}
std::ostream& operator<<(std::ostream& os, const BigInteger& value) {
    os << value.to_string();
    return os;
}
std::istream& operator>>(std::istream& is, BigInteger& value) {
    std::string s;
    is >> s;
    value = BigInteger(s);
    return is;
}