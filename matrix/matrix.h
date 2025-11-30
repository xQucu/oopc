#pragma once

#include <fstream>
#include <iostream>

// Exception classes
class MatrixException {
protected:
    const char* message;

public:
    explicit MatrixException(const char* msg)
        : message(msg)
    {
    }

    virtual ~MatrixException() { }

    virtual const char* what() const { return message; }
};

class DimensionMismatchException : public MatrixException {
public:
    explicit DimensionMismatchException(const char* msg)
        : MatrixException(msg)
    {
    }
};

class IndexOutOfBoundsException : public MatrixException {
public:
    explicit IndexOutOfBoundsException(const char* msg)
        : MatrixException(msg)
    {
    }
};

class FileReadException : public MatrixException {
public:
    explicit FileReadException(const char* msg)
        : MatrixException(msg)
    {
    }
};

class InvalidDimensionException : public MatrixException {
public:
    explicit InvalidDimensionException(const char* msg)
        : MatrixException(msg)
    {
    }
};

// Proxy class for distinguishing read/write operations
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
    // Assignment operator - triggers copy-on-write
    MatrixProxy& operator=(int value);

    // Conversion to int - read operation
    operator int() const;
};

// Matrix class with reference counting
class Matrix {
private:
    int** data;
    int rows;
    int cols;
    int* refCount;

    void detach(); // Copy-on-write mechanism

    friend class MatrixProxy;

public:
    // Constructors and destructor
    Matrix(int rows, int cols);
    Matrix(const Matrix& other); // Copy constructor
    Matrix(std::istream& input); // Read from stream constructor
    ~Matrix();

    // Assignment operator
    Matrix& operator=(const Matrix& other);

    // Accessors
    int getRows() const { return rows; }

    int getCols() const { return cols; }

    int getRefCount() const { return *refCount; }

    // Element access - const version (read-only)
    int operator()(int row, int col) const;

    // Element access - non-const version (potentially write)
    MatrixProxy operator()(int row, int col);

    // Arithmetic operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);

    // Comparison operators
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};
