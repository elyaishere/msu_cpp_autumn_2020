#include "Matrix.h"
#include <stdexcept>
#include <iomanip>

Matrix::Matrix(size_t r, size_t c): rows(r), cols(c) {
    if (r <= 0 || c <= 0)
        throw std::invalid_argument("Некорректный размер матрицы!");
    m = new int[rows * cols]();
}

Matrix::Matrix() {}

Matrix::~Matrix() {
    delete [] m;
}

size_t Matrix::getRows() const {
    return rows;
}

size_t Matrix::getColumns() const {
    return cols;
}

Matrix::proxy::proxy(const Matrix & mat, size_t i) {
    cols = mat.cols;
    m = mat.m + i * cols;
}

int& Matrix::proxy::operator[](size_t j) {
    if (j >= cols || j < 0)
        throw std::out_of_range("Некорректный номер столбца!");
    return m[j];
}

const int& Matrix::proxy::operator[](size_t j) const {
    if (j >= cols || j < 0)
        throw std::out_of_range("Некорректный номер столбца!");
    return m[j];
}

Matrix::proxy  Matrix::operator[](size_t i) {
    if (i >= rows || i < 0)
        throw std::out_of_range("Некорректный номер строки!");
    return proxy(*this, i);
}

Matrix::proxy const   Matrix::operator[](size_t i) const {
    if (i >= rows || i < 0)
        throw std::out_of_range("Некорректный номер строки!");
    return proxy(*this, i);
}


Matrix & Matrix::operator*=(int num) {
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
                (*this)[i][j] *= num;
    return *this;
}

std::ostream& operator<<(std::ostream & out, const Matrix & m) {
    if (m.rows * m.cols == 0) return out;
    out.width(8);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            out.width(8);
            out << m[i][j];
        }
        out<<std::endl;
    }
    return out;
}

Matrix Matrix::operator+(const Matrix & other) const{
    if (other.getColumns() != cols || other.getRows() != rows)
        throw std::invalid_argument("Матрицы различного размера!");
    Matrix res(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            res[i][j] = (*this)[i][j] + other[i][j];
    return res;
}

bool Matrix::operator==(const Matrix & other) const {
    if (other.getColumns() != cols || other.getRows() != rows)
        return false;
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            if ((*this)[i][j] != other[i][j])
                return false;
    return true;
}
bool Matrix::operator!=(const Matrix & other) const {
    return !(*this == other);
}

Matrix Matrix::operator=(const Matrix & other) {
    delete[] m;

    rows = other.getRows();
    cols = other.getColumns();
    m = new int[rows * cols]();

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            (*this)[i][j] = other[i][j];

    return *this;
}
    
Matrix::Matrix(const Matrix & other): rows(other.getRows()), cols(other.getColumns()) {
    delete[] m;
    m = new int[rows * cols]();

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            (*this)[i][j] = other[i][j];
}
