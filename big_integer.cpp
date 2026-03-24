#include <big_integer.h>
BigInteger::BigInteger(){
    digits_={0};
    negative_= false;
}
BigInteger::BigInteger(int value){
    if (value < 0) {
        negative_=true;
        value = -value;
    }
    else {
        negative_ = false;
    }
    do {
        digits_.push_back(value%10);
        value /= 10;
    } while (value > 0);
}
BigInteger::BigInteger(long long value){
    if (value < 0) {
        negative_=true;
        value = -value;
    }
    else {
        negative_ = false;
    }
    do {
        digits_.push_back(value%10);
        value /= 10;
    } while (value > 0);
}
BigInteger::BigInteger(const std::string& str){
    size_t start = 0;
    if (str[0] == '-'){
        negative_ = true;
        start = 1;
    }
    for (int i = str.length()-1;i>=(int)start;--i){
        digits_.push_back(str[i]-'0');
    }
    while (digits_.size()>1 && digits_.back() == '0'){
        digits_.pop_back();
    }
}
std::string BigInteger::to_string() const{
    std::string result;
    if (negative_){
        result+='-';
    }
    for (int i=digits_.size()-1;i >= 0; --i){
        result+= static_cast<char>(digits_[i]+'0');
    }
    return result;
}
bool BigInteger::is_zero() const{
    return digits_.size() == 1 && digits_[0]==0;
}
bool BigInteger::is_negative() const{
    return negative_;
}
BigInteger::operator bool() const{
    return!is_zero();
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
    for (int i = digits_.size() - 1; i > -1; --i){
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
