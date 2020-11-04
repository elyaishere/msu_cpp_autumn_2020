#include "BigInt.h"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <cstdlib>

BigInt::BigInt(int n) {
    sign = (n < 0);
    n = abs(n);

    num = new int[block]();
    buffer = block;

    do {
        if (size == buffer) {
            buffer += block;
            auto tmp = new int[buffer]();
            std::copy(num, num + size, tmp);
            delete[] num;
            num = tmp;
        }
        num[size++] = n % base;
        n /= base;
    } while (n > 0);
}

BigInt::~BigInt() {
    delete[] num;
}

BigInt::BigInt (const std::string & s) {
    int sz = s.size();
    if (sz > 0) {
        int i0 = !isdigit(s[0]);
        sign = (s[0] == '-');
        sz -= i0; 

        size = (sz + nbase - 1) / nbase;
        buffer = ((size + block - 1) / block) * block;
        num = new int[buffer]();
        int i = std::max(i0, sz + i0 - nbase);
        int j = 0;
        do {
            auto tmp = s.substr(i, nbase).c_str();
            int t = atoi(tmp);
            num[j++] = t;
            i-=nbase;
        } while (i0 <= i);
        if (i + nbase > i0) {
            auto tmp = s.substr(i0, nbase + i - i0).c_str();
            int t = atoi(tmp);
            num[j++] = t;
        }
    }
}

std::ostream& operator<<(std::ostream& out, const BigInt& bigInt) {
    if (bigInt.size == 0) return out;
    if (bigInt.sign) out << '-';
    out << bigInt.num[bigInt.size - 1];
    for (int i = bigInt.size - 2; i >= 0; --i) {
        out << std::setw(bigInt.nbase) << std::setfill('0');
        out << bigInt.num[i];
    }
    return out;
}

BigInt::BigInt(const BigInt& other) {
    buffer = other.buffer;
    size = other.size;
    sign = other.sign;
    num = new int[buffer]();
    std::copy(other.num, other.num + size, num);
}

BigInt::BigInt(BigInt && other) {
    buffer = other.buffer;
    size = other.size;
    sign = other.sign;
    num = other.num;

    other.num = nullptr;
}

BigInt& BigInt::operator=(const BigInt &other) {
    if (this == &other) 
        return *this;
    
    buffer = other.buffer;
    size = other.size;
    sign = other.sign;

    auto ptr = new int[buffer]();
    std::copy(other.num, other.num + size, ptr);
    delete[] num;
    num = ptr;

    return *this;
}

BigInt& BigInt::operator=(BigInt &&other) {
    if (this == &other)
        return *this;
    
    buffer = other.buffer;
    size = other.size;
    sign = other.sign;

    delete[] num;
    num = other.num;

    other.num = nullptr;

    return *this;
}

bool BigInt::operator==(const BigInt& other) const {
    if (this == &other)
        return true;
    if (sign != other.sign || size != other.size)
        return false;
    for (int i = 0; i < size; ++i)
        if (num[i] != other.num[i])
            return false;
    return true;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (sign != other.sign)
        return sign;
    if (size != other.size)
        return (size > other.size) == sign;
    for (int i = size - 1; i >= 0; --i)
        if (num[i] != other.num[i])
            return (num[i] > other.num[i]) == sign;
    return false;
}

bool BigInt::operator<=(const BigInt& other) const {
    return (*this < other) || (*this == other);
}

bool BigInt::operator>(const BigInt& other) const {
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

BigInt BigInt::operator-() const{
    BigInt result = *this;
    result.sign = !sign;
    return result;
}

BigInt BigInt::operator+(const BigInt& other) const {
    bool op = (sign != other.sign); // 1 - разного знака, 0 - одного знака

    if (op)
        if (sign) 
            return makeDif(other, -(*this));
        else 
            return makeDif(*this, -other);
    
    return makeSum(*this, other); 
}

BigInt BigInt::operator-(const BigInt& other) const {
    bool op = (sign != other.sign); // 1 - разные знаки, 0 - одинаковые
    if (op)
        return makeSum(*this, other);
    if (sign)
        return makeDif(-other, -(*this));
    else
        return makeDif(*this, other);
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt res;
    
    res.sign = (sign != other.sign);
    res.buffer = ((size + other.size + block - 1) / block) * block;
    res.num = new int[res.buffer]();
    int j = 0;
    while (j < other.size) {
        int i = 0;
        int carry = 0;
        for (i = 0; i < size; ++i) {
            res.num[j + i] += (other.num[j] * num[i] + carry);
            carry = res.num[j + i] / base;
            res.num[j + i] %= base;
        }
        while (carry) {
            res.num[j + i] += carry;
            carry = res.num[j + i] / base;
            res.num[j + i] %= base;
            ++i;
        }
        ++j;
    }
    for (j = res.buffer; res.num[j - 1] == 0 && j > 1 ; --j);
    res.size = j;
    return res;
}

BigInt makeSum (const BigInt & a, const BigInt & b) { // return (sign)(a + b)
    BigInt c;
    c.sign = a.sign;
    c.buffer = std::max(a.buffer, b.buffer) + c.block;
    c.num = new int[c.buffer]();
    std::copy(a.num, a.num + a.size, c.num);
    c.size = a.size;

    int carry = 0;
    int i = 0;
    for (i = 0; i < b.size; ++i) {
        c.num[i] += b.num[i] + carry;
        carry = c.num[i] / c.base;
        c.num[i] %= c.base;
    }

    while (carry) {
        c.num[i] += carry;
        carry = c.num[i] / c.base;
        c.num[i] %= c.base;
        ++i;
    }

    c.size = std::max(size_t(i), c.size);
    return c;
}

BigInt makeDif (const BigInt & a, const BigInt & b) { // return (a - b)
    if (b > a) {
        BigInt c;
        c = makeDif(b, a);
        c.sign = 1;
        return c;
    }
    else {
        BigInt c;
        c = a;
        int i = 0;
        while (i < b.size) {
            c.num[i] -= b.num[i];
            if (c.num[i] < 0) {
                c.num[i+1] -= 1;
                c.num[i] += c.base;
            }
            ++i;
        }
        while ((i < c.size - 1) && c.num[i] < 0) {
            c.num[i+1] -= 1;
            c.num[i] += c.base;
            ++i;
        }
        while (c.num[c.size - 1] == 0 && c.size > 1) c.size -= 1; 
        return c;
    }
}

BigInt operator+(const int n, const BigInt& other) {
    return other + n;
}
BigInt operator-(const int n, const BigInt& other) {
    return (- other + n);
}
BigInt operator*(const int n, const BigInt& other) {
    return (other * n);
}
bool operator==(const int n, const BigInt& other) {
    return (other == n);
} 
bool operator!=(const int n, const BigInt& other) {
    return (other != n);
}
bool operator<=(const int n, const BigInt& other) {
    return (other >= n);
}
bool operator>=(const int n, const BigInt& other) {
    return (other <= n);
}
bool operator<(const int n, const BigInt& other) {
    return (other > n);
}
bool operator>(const int n, const BigInt& other) {
    return (other < n);
}
