#pragma once

#include <ostream>

class Complex {
public:
    Complex(double real = 0, double imag = 0);

    Complex operator-() const;
    Complex& operator+=(const Complex& co);
    Complex& operator-=(const Complex& co);
    Complex& operator*=(const Complex& co);
    Complex& operator/=(const Complex& co);

    double getReal() const;
    double getImag() const;
    void setReal(double real);
    void setImag(double imag);
    double amplitude() const;
    double phase() const;

private:
    double real;
    double imag;
};

std::ostream& operator<<(std::ostream& output, const Complex& toOutput);
Complex operator+(const Complex&, const Complex&);
Complex operator-(const Complex&, const Complex&);
Complex operator*(const Complex&, const Complex&);
Complex operator/(const Complex&, const Complex&);
bool operator==(const Complex&, const Complex&);
bool operator!=(const Complex&, const Complex&);

Complex operator+(const Complex&, double);
Complex operator-(const Complex&, double);
Complex operator*(const Complex&, double);
Complex operator/(const Complex&, double);
Complex operator+(double, const Complex&);
Complex operator-(double, const Complex&);
Complex operator*(double, const Complex&);
Complex operator/(double, const Complex&);
