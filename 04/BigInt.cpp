#include <iostream>
#include <math.h>
#include "BigInt.h"

using namespace std;

std::ostream &operator<<(std::ostream &out, const BigInt &value) {
    if (value.is_zero) {
        out << "0";
        return out;
    }
    if (value.is_neg) {
        out << '-';
    }
    for (int i = value.size - 1; i >= 0; i--) {
        out << int(value.mas[i]);
    }
    return out;
}

BigInt::BigInt() : size(1), is_neg(false), is_zero(true) {
    mas = new char[1]();
}

BigInt::BigInt(long long x) {
    is_neg = false;
    if (x < 0) {
        x = -x;
        is_neg = true;
    }
    is_zero = false;
    if (x == 0) {
        mas = new char[1]();
        size = 1;
        is_zero = true;
    } else {
        size = 0;
        mas = new char[20]();
        while (x > 0) {
            mas[size] = x % 10;
            size++;
            x /= 10;
        }
    }
}

BigInt::BigInt(char *m, size_t s, bool isn) : mas(m), size(s), is_neg(isn) {
    is_zero = ((size == 1) && (mas[0] == 0));
}

BigInt::~BigInt() {
    delete[] mas;
}

BigInt::BigInt(const BigInt &value) : mas(new char[value.size]), is_neg(value.is_neg), size(value.size), is_zero(value.is_zero) {
    std::copy(value.mas, value.mas + size, mas);
}

BigInt &BigInt::operator=(const BigInt &value) {
    if (value == *this) {
        return *this;
    }
    char *tmp = new char[value.size];
    delete[] mas;
    mas = tmp;
    size = value.size;
    is_zero = value.is_zero;
    std::copy(value.mas, value.mas + size, mas);
    return *this;
}

BigInt::BigInt(BigInt &&value) : size(std::move(value.size)), is_neg(std::move(value.is_neg)), is_zero(std::move(value.is_zero)) {
    delete[] mas;
    mas = std::move(value.mas);
    value.mas = nullptr;
    value.size = 0;
}

BigInt &BigInt::operator=(BigInt &&value) {
    if (*this == value) {
        return *this;
    }
    delete[] mas;
    mas = std::move(value.mas);
    size = std::move(value.size);
    is_neg = std::move(value.is_neg);
    is_zero = std::move(value.is_zero);
    value.mas = nullptr;
    value.size = 0;
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt tmp(*this);
    tmp.is_neg = !tmp.is_neg;
    return tmp;
}

bool BigInt::operator==(const BigInt &value) const {
    if (size != value.size) {
        return false;
    }
    if (is_zero && value.is_zero) {
        return true;
    }
    if (is_neg != value.is_neg) {
        return false;
    }
    for (int i = size - 1; i >= 0; i--) {
        if (mas[i] != value.mas[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt &value) const {
    return !(*this == value);
}

bool BigInt::operator<=(const BigInt &value) const {
    if (value == *this) {
        return true;
    }
    if (value.is_neg && !is_neg) {
        return false;
    }
    if (!value.is_neg && is_neg) {
        return true;
    }
    if (!value.is_neg && !is_neg) {
        if (value.size > size) {
            return true;
        }
        if (value.size < size) {
            return false;
        }
        for (int i = size - 1; i >= 0; i--) {
            if (mas[i] > value.mas[i]) {
                return false;
            } else if (mas[i] < value.mas[i]) {
                return true;
            }
        }
        return true;
    } else if (value.is_neg && is_neg) {
        if (value.size > size) {
            return false;
        }
        if (value.size < size) {
            return true;
        }
        for (int i = size - 1; i >= 0; i--) {
            if (mas[i] > value.mas[i]) {
                return true;
            } else if (mas[i] < value.mas[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool BigInt::operator>(const BigInt &value) const {
    return !(*this <= value);
}

bool BigInt::operator>=(const BigInt &value) const {
    if (value == *this) {
        return true;
    }
    if (value.is_neg && !is_neg) {
        return true;
    }
    if (!value.is_neg && is_neg) {
        return false;
    }
    if (!value.is_neg && !is_neg) {
        if (value.size > size) {
            return false;
        }
        if (value.size < size) {
            return true;
        }
        for (int i = size - 1; i >= 0; i--) {
            if (mas[i] > value.mas[i]) {
                return true;
            } else if (mas[i] < value.mas[i]) {
                return false;
            }
        }
        return true;
    } else if (value.is_neg && is_neg) {
        if (value.size > size) {
            return true;
        }
        if (value.size < size) {
            return false;
        }
        for (int i = size - 1; i >= 0; i--) {
            if (mas[i] > value.mas[i]) {
                return false;
            } else if (mas[i] < value.mas[i]) {
                return true;
            }
        }
        return true;
    }
    return false;
}

bool BigInt::operator<(const BigInt &value) const {
    return !(*this >= value);
}

BigInt BigInt::operator+(BigInt value) const {
    if (!is_neg && value.is_neg) {
        auto tmp = -value;
        return (*this).operator-(tmp);
    }
    if (is_neg && !value.is_neg) {
        auto tmp = -(*this);
        return value.operator-(tmp);
    }
    int max_size = 0;
    int min_size = 0;
    if (size >= value.size) {
        max_size = size;
        min_size = value.size;
    } else {
        max_size = value.size;
        min_size = size;
    }
    char pere = 0;
    char *res = new char[max_size + 1]();
    int i = 0;
    for (; i < min_size; i++) {
        res[i] = (mas[i] + value.mas[i] + pere) % 10;
        pere = (mas[i] + value.mas[i] + pere) / 10;
    }
    for (; i < max_size; i++) {
        if (max_size == size) {
            res[i] = (mas[i] + pere) % 10;
            pere = (mas[i] + pere) / 10;
        } else {
            res[i] = (value.mas[i] + pere) % 10;
            pere = (value.mas[i] + pere) / 10;
        }
    }
    int res_size = max_size;
    if (pere > 0) {
        res[i] = pere;
        res_size++;
    }
    return BigInt(res, res_size, is_neg);
}

BigInt BigInt::operator-(BigInt value) const {
    if (!is_neg && value.is_neg) {
        auto tmp = -value;
        return (*this).operator+(tmp);
    }
    if (is_neg && !value.is_neg) {
        auto tmp = -(*this);
        return -(value.operator+(tmp));
    }
    if (is_neg && value.is_neg) {
        auto tmp1 = -(*this);
        auto tmp2 = -value;
        return -(tmp1 - tmp2);
    }
    if (size > value.size) {
        char *res = new char[size]();
        int pere = 0;
        int i = 0;
        for (; i < value.size; i++) {
            res[i] = mas[i] - pere - value.mas[i];
            pere = 0;
            if (res[i] < 0) {
                res[i] += 10;
                pere = 1;
            }
        }
        for (;i < size; i++) {
            res[i] = mas[i] - pere;
            pere = 0;
            if (res[i] < 0) {
                res[i] += 10;
                pere = 1;
            }
        }
        i--;
        while (res[i] == 0) {
            i--;
        }
        return BigInt(res, i + 1, false);
    } else if (value.size > size) {
        char *res = new char[value.size]();
        int pere = 0;
        int i = 0;
        for (; i < size; i++) {
            res[i] = value.mas[i] - pere - mas[i];
            pere = 0;
            if (res[i] < 0) {
                res[i] += 10;
                pere = 1;
            }
        }
        for (;i < value.size; i++) {
            res[i] = value.mas[i] - pere;
            pere = 0;
            if (res[i] < 0) {
                res[i] += 10;
                pere = 1;
            }
        }
        i--;
        while (res[i] == 0) {
            i--;
        }
        return BigInt(res, i + 1, true);
    } else {
        if (*this == value) {
            return BigInt(0);
        }
        if (*this > value) {
            char *res = new char[size]();
            int pere = 0;
            int i = 0;
            for (; i < size; i++) {
                res[i] = mas[i] - pere - value.mas[i];
                pere = 0;
                if (res[i] < 0) {
                    res[i] += 10;
                    pere = 1;
                }
            }
            i--;
            while (res[i] == 0) {
                i--;
            }
            return BigInt(res, i + 1, false);
        } else {
            char *res = new char[size]();
            int pere = 0;
            int i = 0;
            for (; i < size; i++) {
                res[i] = value.mas[i] - pere - mas[i];
                pere = 0;
                if (res[i] < 0) {
                    res[i] += 10;
                    pere = 1;
                }
            }
            i--;
            while (res[i] == 0) {
                i--;
            }
            return BigInt(res, i + 1, true);
        }
    }
}



