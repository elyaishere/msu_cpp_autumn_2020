#include "BigInt.h"
#include <cassert>

int main() {
    BigInt a = 1;
    BigInt b("123456789012345678901234567890");

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    BigInt c = a * b + 2;
    std::cout << "c = " << c <<std::endl;

    BigInt d;
    d = std::move(c);
    std::cout << "d = " << d <<std::endl;

    a = d + b;
    std::cout << "a = " << a <<std::endl;

    return 0;
}