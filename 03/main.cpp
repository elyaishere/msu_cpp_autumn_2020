#include "Matrix.h"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

int main() {
    const size_t rows = 5;
    const size_t cols = 3;
    std::stringstream ss;

    Matrix m(rows, cols);
    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);

    m[1][2] = 5;
    int x = m[1][2];
    assert(x == 5);

    try {
        m[0][cols] = 8;
    } catch (std::exception& e) {
        ss << "Fail: " << e.what() << std::endl;
    }

    try {
        m[-2][1] = 8;
    } catch (std::exception& e) {
        ss << "Fail: " << e.what() << std::endl;
    }

    m *= 3;
    assert(m[1][2] == 15);

    Matrix m1(rows, cols);

    if (m == m1) {
         ss << "Wrong!\n";
    }

    if (m != m1) {
        ss << "Ok!\n";
        Matrix m2 = m1 + m;
        ss << m2;
    }
    
    Matrix m2(m);

    if (m2 == m) {
        ss << "Конструктор копирования ок\n";
    }

    m2 = m1;

    if (m2 == m1) {
        ss << "Оператор копирования ок\n";
    }

    Matrix m3;
    m3 = m;

    if (m3 == m) {
        ss << "Конструктор по умолчанию ок\n";
    }

    std::cout << "*****\n" << "Журнал работы:\n" << ss.str() << "*****\n";
    return 0;
}
