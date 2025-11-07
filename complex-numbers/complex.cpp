#include "complex.h"
#include <cmath>
#include <limits.h>
#include <stdlib.h>

Complex::Complex(double real, double imag)
{
    this->real = real;
    this->imag = imag;
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

Complex operator/(const Complex& c1, const Complex& c2)
{
    double denom = c2.getReal() * c2.getReal() + c2.getImag() * c2.getImag();
    if (denom == 0) {
        throw std::overflow_error("Division by zero");
    }

    Complex tmp;
    tmp.setReal((c1.getReal() * c2.getReal() + c1.getImag() * c2.getImag()) / denom);
    tmp.setImag((c1.getImag() * c2.getReal() - c1.getReal() * c2.getImag()) / denom);
    return tmp;
}

Complex& Complex::operator+=(const Complex& co)
{
    this->real += co.getReal();
    this->imag += co.getImag();
    return *this;
}

Complex& Complex::operator-=(const Complex& co)
{
    this->real -= co.getReal();
    this->imag -= co.getImag();
    return *this;
}

Complex& Complex::operator*=(const Complex& co)
{
    double newReal = this->getReal() * co.getReal() - this->getImag() * co.getImag();
    double newImag = this->getReal() * co.getImag() + this->getImag() * co.getReal();

    this->setReal(newReal);
    this->setImag(newImag);
    return *this;
}

Complex& Complex::operator/=(const Complex& co)
{
    double denom = co.getReal() * co.getReal() + co.getImag() * co.getImag();
    if (denom == 0) {
        throw "Division by zero";
    }

    double newReal = (this->getReal() * co.getReal() + this->getImag() * co.getImag()) / denom;
    double newImag = (this->getImag() * co.getReal() - this->getReal() * co.getImag()) / denom;

    this->setReal(newReal);
    this->setImag(newImag);

    return *this;
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

double Complex::amplitude() const
{
    return std::sqrt(this->real * this->real + this->imag * this->imag);
}

double Complex::phase() const
{
    return atan2(this->imag, this->real);
}

Complex operator+(const Complex& c, double s)
{
    return Complex(c.getReal() + s, c.getImag());
}

Complex operator-(const Complex& c, double s)
{
    return Complex(c.getReal() - s, c.getImag());
}

Complex operator*(const Complex& c, double s)
{
    return Complex(c.getReal() * s, c.getImag() * s);
}

Complex operator/(const Complex& c, double s)
{
    if (s == 0) {
        throw std::overflow_error("Division by zero");
    }
    return Complex(c.getReal() / s, c.getImag() / s);
}

Complex operator+(double s, const Complex& c)
{
    return Complex(s + c.getReal(), c.getImag());
}

Complex operator-(double s, const Complex& c)
{
    return Complex(s - c.getReal(), -c.getImag());
}

Complex operator*(double s, const Complex& c)
{
    return Complex(s * c.getReal(), s * c.getImag());
}

Complex operator/(double s, const Complex& c)
{
    double denom = c.getReal() * c.getReal() + c.getImag() * c.getImag();
    if (denom == 0) {
        throw std::overflow_error("Division by zero");
    }
    return Complex(s * c.getReal() / denom, -s * c.getImag() / denom);
}
