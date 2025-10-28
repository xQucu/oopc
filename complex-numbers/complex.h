#pragma once

#include <ostream>

class Complex {
public:
    Complex(double real = 0, double imag = 0);
    Complex(double co);


    friend std::ostream& operator<<(std::ostream& output, const Complex& toOutput);
    friend Complex operator+(const Complex&, const Complex&);
    friend Complex operator-(const Complex&, const Complex&);
    Complex operator-() const;
    friend Complex operator*(const Complex&, const Complex&);
    friend Complex operator/(const Complex&, const Complex&);
    Complex& operator+=(const Complex co);
    Complex& operator-=(const Complex co);
    Complex& operator*=(const Complex co);
    Complex& operator/=(const Complex co);
    friend bool operator==(const Complex&, const Complex&);
    friend bool operator!=(const Complex&, const Complex&);

    double getReal() const;
    double getImag() const;
    void setReal(double real);
    void setImag(double imag);
    double amplitude();
    double phase();

private:
    double real;
    double imag;
};
