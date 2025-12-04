#pragma once

#include <fstream>
#include <iostream>
#include <string>

class MatrixException {
private:
    std::string message;

public:
    MatrixException(const char* msg)
    {
        message.assign(msg);
    }

    virtual ~MatrixException() = default;

    virtual const std::string what() const { return message; }
};

class DimensionMismatchException : public MatrixException {
public:
    DimensionMismatchException(const char* msg)
        : MatrixException(msg)
    {
    }
};

class IndexOutOfBoundsException : public MatrixException {
public:
    IndexOutOfBoundsException(const char* msg)
        : MatrixException(msg)
    {
    }
};

class FileReadException : public MatrixException {
public:
    FileReadException(const char* msg)
        : MatrixException(msg)
    {
    }
};

class Matrix;

class MatrixProxy {
private:
    Matrix& matrix;
    int row;
    int col;
    friend class Matrix;

    MatrixProxy(Matrix& m, int r, int c)
        : matrix(m)
        , row(r)
        , col(c)
    {
    }

public:
    MatrixProxy& operator=(int value);

    operator int() const;
};

class Matrix {
private:
    int** data;
    int rows;
    int cols;
    int* refCount;

    void detach();

    friend class MatrixProxy;

public:
    Matrix(int rows, int cols);
    Matrix(const Matrix& other);
    Matrix(std::istream& input);
    ~Matrix();

    Matrix& operator=(const Matrix& other);

    int getRows() const { return rows; }

    int getCols() const { return cols; }

    int getRefCount() const { return *refCount; }

    int operator()(int row, int col) const;

    MatrixProxy operator()(int row, int col);

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};
