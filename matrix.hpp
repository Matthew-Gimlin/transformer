#pragma once

#include <sstream>
#include <initializer_list>

class Matrix
{
public:
    Matrix();
    Matrix(unsigned rows, unsigned columns);
    Matrix(std::initializer_list<std::initializer_list<float>> list);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    
    const unsigned rowSize() const;
    const unsigned columnSize() const;
    const float* const* const data() const;

    float* operator[](unsigned row);
    const float* const operator[](unsigned row) const;
    float& at(unsigned row, unsigned column);
    const float& at(unsigned row, unsigned column) const;

    Matrix operator*(const Matrix& matrix) const;
    Matrix operator*(const float scalar) const;
    Matrix operator+(const Matrix& matrix) const;
    Matrix operator+(const float scalar) const;
    Matrix operator-(const Matrix& matrix) const;
    Matrix operator-(const float scalar) const;
    Matrix dot(const Matrix& matrix) const;
    Matrix transpose() const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

private:
    unsigned rows;
    unsigned columns;
    float** elements;
    
    void allocate(unsigned rows, unsigned columns);
    void free();
};
