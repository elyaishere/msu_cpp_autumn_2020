#pragma once
#include <cstddef>
#include <iostream>
#include <string>

class BigInt {
    public:
    public:
    BigInt(int);
    BigInt(const BigInt &);
    BigInt(BigInt &&);
    BigInt(const std::string&);
    BigInt(){}
    ~BigInt();

    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt &&);
    BigInt operator-() const;
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator>=(const BigInt&) const;
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    friend BigInt makeSum (const BigInt &, const BigInt &);
    friend BigInt makeDif (const BigInt &, const BigInt &);
    friend BigInt operator+(const int, const BigInt&);
    friend BigInt operator-(const int, const BigInt&);
    friend BigInt operator*(const int, const BigInt&);
    friend bool operator==(const int, const BigInt&);
    friend bool operator!=(const int, const BigInt&);
    friend bool operator<=(const int, const BigInt&);
    friend bool operator>=(const int, const BigInt&);
    friend bool operator<(const int, const BigInt&);
    friend bool operator>(const int, const BigInt&);

    private:
    size_t size = 0;
    int * num = nullptr;
    bool sign = 0;
    size_t buffer = 0;
    static const size_t block = 5;
    static const int base = 10000;
    static const int nbase = 4;
};
