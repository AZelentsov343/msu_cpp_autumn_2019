#ifndef INC_04_BIGINT_H
#define INC_04_BIGINT_H

class BigInt {
    char *mas;
    size_t size;
    bool is_neg;
    bool is_zero;
public:
    BigInt(char *m, size_t s, bool isn);
    BigInt(long long x);
    BigInt();
    ~BigInt();
    BigInt(const BigInt &value);
    BigInt &operator=(const BigInt &value);
    BigInt(BigInt&& value);
    BigInt &operator=(BigInt&& value);
    friend std::ostream& operator<<(std::ostream& out, const BigInt& value);
    BigInt operator+(BigInt value) const;
    BigInt operator-(BigInt value) const;
    BigInt operator-() const;
    bool operator==(const BigInt& value) const;
    bool operator!=(const BigInt& value) const;
    bool operator>=(const BigInt& value) const;
    bool operator<=(const BigInt& value) const;
    bool operator<(const BigInt& value) const;
    bool operator>(const BigInt& value) const;
};




#endif //INC_04_BIGINT_H
