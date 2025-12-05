#include "matrix.h"
#include <cstring>

Matrix::Matrix(int r, int c)
    : data(0)
    , rows(r)
    , cols(c)
    , refCount(0)
{
    if (r <= 0 || c <= 0) {
        throw DimensionMismatchException("Matrix dimensions must be positive");
    }

    refCount = new int(1);

    data = new int*[rows];

    int* block = new int[rows * cols];

    for (int i = 0; i < rows; ++i) {
        data[i] = block + i * cols;
        for (int j = 0; j < cols; ++j) {
            data[i][j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix& other)
    : data(other.data)
    , rows(other.rows)
    , cols(other.cols)
    , refCount(other.refCount)
{
    (*refCount)++;
}

Matrix::Matrix(std::istream& input)
    : data(0)
    , rows(0)
    , cols(0)
    , refCount(0)
{
    int r, c;
    if (!(input >> r >> c)) {
        throw FileReadException("Failed to read matrix dimensions");
    }

    if (r <= 0 || c <= 0) {
        throw DimensionMismatchException("Matrix dimensions must be positive");
    }

    rows = r;
    cols = c;
    refCount = new int(1);

    data = new int*[rows];
    int* block = new int[rows * cols];

    for (int i = 0; i < rows; ++i) {
        data[i] = block + i * cols;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(input >> data[i][j])) {
                delete[] data[0];
                delete[] data;
                delete refCount;
                throw FileReadException("Failed to read matrix element");
            }
        }
    }
}

Matrix::~Matrix()
{
    if (refCount) {
        (*refCount)--;
        if (*refCount == 0) {
            if (data) {
                delete[] data[0];
                delete[] data;
            }
            delete refCount;
        }
    }
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this != &other) {
        if (refCount) {
            (*refCount)--;
            if (*refCount == 0) {
                if (data) {
                    delete[] data[0];
                    delete[] data;
                }
                delete refCount;
            }
        }

        data = other.data;
        rows = other.rows;
        cols = other.cols;
        refCount = other.refCount;
        (*refCount)++;
    }
    return *this;
}

void Matrix::detach()
{
    if (*refCount > 1) {
        int** oldData = data;

        data = new int*[rows];
        int* block = new int[rows * cols];

        for (int i = 0; i < rows; ++i) {
            data[i] = block + i * cols;
        }

        memcpy(data[0], oldData[0], rows * cols * sizeof(int));

        (*refCount)--;

        refCount = new int(1);
    }
}

int Matrix::operator()(int row, int col) const
{
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw IndexOutOfBoundsException("Index out of bounds");
    }
    return data[row][col];
}

MatrixProxy Matrix::operator()(int row, int col)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw IndexOutOfBoundsException("Index out of bounds");
    }
    return MatrixProxy(*this, row, col);
}

MatrixProxy& MatrixProxy::operator=(int value)
{
    matrix.detach();
    matrix.data[row][col] = value;
    return *this;
}

MatrixProxy::operator int() const
{
    return matrix.data[row][col];
}

Matrix Matrix::operator+(const Matrix& other) const
{
    if (rows != other.rows || cols != other.cols) {
        throw DimensionMismatchException("Matrix dimensions must match for addition");
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const
{
    if (rows != other.rows || cols != other.cols) {
        throw DimensionMismatchException("Matrix dimensions must match for subtraction");
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const
{
    if (cols != other.rows) {
        throw DimensionMismatchException("Invalid dimensions for matrix multiplication");
    }

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            int sum = 0;
            for (int k = 0; k < cols; ++k) {
                sum += data[i][k] * other.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    if (rows != other.rows || cols != other.cols) {
        throw DimensionMismatchException("Matrix dimensions must match for addition");
    }

    detach();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other)
{
    if (rows != other.rows || cols != other.cols) {
        throw DimensionMismatchException("Matrix dimensions must match for subtraction");
    }

    detach();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
    *this = *this * other;
    return *this;
}

bool Matrix::operator==(const Matrix& other) const
{
    if (rows != other.rows || cols != other.cols) {
        return false;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = 0; j < matrix.cols; ++j) {
            os << matrix.data[i][j];
            if (j < matrix.cols - 1) {
                os << " ";
            }
        }
        if (i < matrix.rows - 1) {
            os << "\n";
        }
    }
    return os;
}
