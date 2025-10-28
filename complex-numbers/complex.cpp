#include "complex.h"
#include <cmath>
#include <limits.h>
#include <stdlib.h>

Complex::Complex(double real, double imag)
{
    this->real = real;
    this->imag = imag;
}

Complex::Complex(double co)
{
    this->real = co;
    this->imag = 0;
}

std::ostream& operator<<(std::ostream& output, const Complex& complex)
{
    output << "(" << complex.getReal() << "," << complex.getImag() << ")";
    return output;
}

Complex operator+(const Complex& c1, const Complex& c2)
{
    Complex tmp;
    tmp.setReal(c1.getReal() + c2.getReal());
    tmp.setImag(c1.getImag() + c2.getImag());
    return tmp;
}

Complex operator-(const Complex& c1, const Complex& c2)
{
    Complex tmp;
    tmp.setReal(c1.getReal() - c2.getReal());
    tmp.setImag(c1.getImag() - c2.getImag());
    return tmp;
}

Complex Complex::operator-() const
{
    return Complex(-(this->getReal()), -(this->getImag()));
}

Complex operator*(const Complex& c1, const Complex& c2)
{
    // (a+ib)(c+id)=ac+iad+icb-bd=ac-bd+i(ad+cb)
    Complex tmp;
    tmp.setReal(c1.getReal() * c2.getReal() - c1.getImag() * c2.getImag());
    tmp.setImag(c1.getReal() * c2.getImag() + c2.getReal() * c1.getImag());
    return tmp;
}

Complex operator/(Complex&, Complex&)
{
}

Complex& Complex::operator+=(Complex co)
{
    this->real += co.getReal();
    this->imag += co.getImag();
    return *this;
}

Complex& Complex::operator-=(Complex co)
{
    this->real -= co.getReal();
    this->imag -= co.getImag();
    return *this;
}

Complex& Complex::operator*=(Complex co)
{
    this->real *= co.getReal();
    this->imag *= co.getImag();
    return *this;
}

Complex& Complex::operator/=(Complex co)
{
}

bool operator==(const Complex& c1, const Complex& c2)
{
    return c1.getReal() == c2.getReal() && c1.getImag() == c2.getImag();
}

bool operator!=(const Complex& c1, const Complex& c2)
{

    return c1.getReal() != c2.getReal() || c1.getImag() != c2.getImag();
}

double Complex::getReal() const
{
    return this->real;
}

double Complex::getImag() const
{
    return this->imag;
}

void Complex::setReal(double real)
{
    this->real = real;
}

void Complex::setImag(double imag)
{
    this->imag = imag;
}

double Complex::amplitude()
{
    return std::sqrt(this->real * this->real + this->imag * this->imag);
}

double Complex::phase()
{
    return atan2(this->imag, this->real);
}
