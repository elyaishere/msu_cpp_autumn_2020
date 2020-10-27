#pragma once
#include <cstddef>
#include <iostream>

class Matrix {
    public:
    Matrix(size_t, size_t);
    ~Matrix();
    size_t getRows() const;
    size_t getColumns() const;

    class proxy {
        public:
        proxy(const Matrix &, size_t);
        int& operator[](size_t);
        const int& operator[](size_t) const;

        private:
        size_t cols;
        int * m;
    };

    proxy operator[](size_t);
    proxy const operator[](size_t) const;
    Matrix & operator*=(int);
    friend std::ostream& operator<<(std::ostream &, const Matrix &);
    Matrix operator+(const Matrix &) const;
    bool operator==(const Matrix &) const;
    bool operator!=(const Matrix &) const;

    Matrix operator=(const Matrix &);
    Matrix(const Matrix &);
    Matrix();

    private:
    size_t rows = 0;
    size_t cols = 0;
    int * m = nullptr;
};
