#include "matrix.hpp"

#include <sstream>
#include <stdexcept>
#include <algorithm>

Matrix::Matrix()
{
    this->rows = 0;
    this->columns = 0;
    this->elements = nullptr;
}

Matrix::Matrix(unsigned rows, unsigned columns)
{
    this->allocate(rows, columns);
}

Matrix::Matrix(std::initializer_list<std::initializer_list<float>> list)
{
    unsigned rows = list.size();
    unsigned columns = list.begin()->size();
    
    this->allocate(rows, columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        if (list.begin()[i].size() != columns)
        {
            std::stringstream ss;
            ss << "Inconsistent dimensions. Expected "
                << columns 
                << " but got "
                << list.begin()[i].size();
            
            throw std::invalid_argument(ss.str());
        }
        
        for (unsigned j = 0; j < this->columns; ++j)
        {
            this->elements[i][j] = list.begin()[i].begin()[j];
        }
    }
}

Matrix::Matrix(const Matrix& other)
{
    this->allocate(other.rows, other.columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            this->elements[i][j] = other[i][j];
        }
    }
}

Matrix::Matrix(Matrix&& other)
{
    this->rows = other.rows;
    this->columns = other.columns;
    this->elements= other.elements;

    other.rows = 0;
    other.columns = 0;
    other.elements = nullptr;
}

Matrix::~Matrix()
{
    this->free();
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->free();

    this->allocate(other.rows, other.columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            this->elements[i][j] = other[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator=(Matrix&& other)
{
    this->free();

    this->rows = other.rows;
    this->columns = other.columns;
    this->elements= other.elements;

    other.rows = 0;
    other.columns = 0;
    other.elements = nullptr;
    
    return *this;
}

const unsigned Matrix::rowSize() const
{
    return this->rows;
}

const unsigned Matrix::columnSize() const
{
    return this->columns;
}

const float* const* const Matrix::data() const
{
    return this->elements;
}

float* Matrix::operator[](unsigned row)
{
    return this->elements[row];
}

const float* const Matrix::operator[](unsigned row) const
{
    return this->elements[row];
}

float& Matrix::at(unsigned row, unsigned column)
{
    if (row >= this->rows || column >= this->columns)
    {
        throw std::out_of_range("");
    }
    
    return this->elements[row][column];
}

const float& Matrix::at(unsigned row, unsigned column) const
{
    if (row >= this->rows || column >= this->columns)
    {
        throw std::out_of_range("");
    }
    
    return this->elements[row][column];
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    if (this->rows != matrix.rows && this->columns != matrix.rows)
    {
        throw std::invalid_argument("");
    }
    
    Matrix result(this->rows, this->columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            result.elements[i][j] = this->elements[i][j] * matrix.elements[i][j];
        }
    }
    
    return result;
}

Matrix Matrix::operator*(const float scalar) const
{
    Matrix result(this->rows, this->columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            result.elements[i][j] = this->elements[i][j] * scalar;
        }
    }
    
    return result;
}

Matrix Matrix::operator+(const Matrix& matrix) const
{
    if (this->rows != matrix.rows && this->columns != matrix.rows)
    {
        throw std::invalid_argument("");
    }
    
    Matrix result(this->rows, this->columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            result.elements[i][j] = this->elements[i][j] + matrix.elements[i][j];
        }
    }
    
    return result;
}

Matrix Matrix::operator+(const float scalar) const
{
    Matrix result(this->rows, this->columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            result.elements[i][j] = this->elements[i][j] + scalar;
        }
    }
    
    return result;
}

Matrix Matrix::operator-(const Matrix& matrix) const
{
    if (this->rows != matrix.rows && this->columns != matrix.rows)
    {
        throw std::invalid_argument("");
    }
    
    Matrix result(this->rows, this->columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            result.elements[i][j] = this->elements[i][j] - matrix.elements[i][j];
        }
    }
    
    return result;
}

Matrix Matrix::operator-(const float scalar) const
{
    Matrix result(this->rows, this->columns);
    for (unsigned i = 0; i < this->rows; ++i)
    {
        for (unsigned j = 0; j < this->columns; ++j)
        {
            result.elements[i][j] = this->elements[i][j] - scalar;
        }
    }
    
    return result;
}

Matrix Matrix::dot(const Matrix& matrix) const
{
    if (this->columns != matrix.rows)
    {
        std::stringstream ss;
        ss << "Cannot multiply matrices ("
            << this->rows
            << ", "
            << this->columns
            << ") and ("
            << matrix.rows
            << ", "
            << matrix.columns
            << ").";
        
        throw std::invalid_argument(ss.str());
    }
    
    Matrix result(this->rows, matrix.columns);
    for (unsigned i = 0; i < result.rows; ++i)
    {
        for (unsigned j = 0; j < result.columns; ++j)
        {
            result.elements[i][j] = 0.0f;
            for (unsigned k = 0; k < matrix.columns; ++k)
            {
                result.elements[i][j] += this->elements[i][k] * matrix.elements[k][j];
            }
        }
    }
    
    return result;
}

Matrix Matrix::transpose() const
{
    Matrix result(this->columns, this->rows);
    for (unsigned i = 0; i < result.rows; ++i)
    {
        for (unsigned j = 0; j < result.columns; ++j)
        {
            result.elements[i][j] = this->elements[j][i];
        }
    }
    
    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    for (unsigned i = 0; i < matrix.rows; ++i)
    {
        os << "[ ";
        for (unsigned j = 0; j < matrix.columns; ++j)
        {
            os << matrix.elements[i][j] << ' ';
        }
        os << "]\n";
    }
    
    return os;
}

void Matrix::allocate(unsigned rows, unsigned columns)
{
    this->rows = rows;
    this->columns = columns;
    this->elements = new float*[this->rows];
    for (unsigned i = 0; i < this->rows; ++i)
    {
        this->elements[i] = new float[this->columns]{0.0f};
    }
}

void Matrix::free()
{
    for (unsigned i = 0; i < this->rows; ++i)
    {
        delete[] this->elements[i];
    }
    delete[] this->elements;
    
    this->rows = 0;
    this->columns = 0;
    this->elements = nullptr;
}
