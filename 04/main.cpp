#include "BigInt.h"
#include <cassert>

int main() {
    BigInt a = 1;
    BigInt b("123456789012345678901234567890");

    assert (a != b);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    BigInt c = a * b + 2;
    std::cout << "c = " << c <<std::endl;

    BigInt d;
    d = std::move(c);
    std::cout << "d = " << d <<std::endl;

    a = d + b;
    std::cout << "a = " << a <<std::endl;

    std::cout << "a - a = " << (a - a) << std::endl;
    std::cout << "a - b = " << (a - b) << std::endl;
    std::cout << "b - d = " << (b - d) << std::endl;

    std::cout << "500000000 + (1) = " << BigInt(500000000) + (1) << std::endl;
    std::cout << "500000000 + (-1) = " << BigInt(500000000) + (-1) << std::endl;
    std::cout << "-500000000 + (-1) = " << BigInt(-500000000) + (-1) << std::endl;
    std::cout << "-500000000 + (1) = " << BigInt(-500000000) + (1) << std::endl;

    std::cout << "500000000 !=  (-1) = " << (BigInt(500000000) != BigInt(-1)) << std::endl;
    std::cout << "-1 != -1 = " << (BigInt(-1) != BigInt(-1)) << std::endl;

    std::cout << "1111 * 1111 = " << BigInt(1111) * 1111 << std::endl;
    std::cout << "1111 * (-1111) = " << BigInt(1111) * (-1111) << std::endl;

    

    return 0;
}
