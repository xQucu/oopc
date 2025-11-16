#include "poly.h"
#include <cstdlib>
#include <iostream>

Poly::Poly(double val)
{
    this->values = std::map<int, double>();
    this->values[0] = val;
}

double& Poly::operator[](int key)
{
    return this->values[key];
}

double Poly::operator[](int key) const
{
    return this->values.at(key);
}

void Poly::addValuesFromPoly(const Poly& p)
{
    for (const auto& [key, value] : p.values) {
        if (value != 0) {
            this->values[key] += value;
        }
    }
}

Poly operator+(const Poly& p1, const Poly& p2)
{
    Poly poly = Poly();
    poly.addValuesFromPoly(p1);
    poly.addValuesFromPoly(p2);
    return poly;
}

void Poly::substractValuesFromPoly(const Poly& p)
{
    for (const auto& [key, value] : p.values) {
        if (value != 0) {
            this->values[key] -= value;
        }
    }
}

Poly operator-(const Poly& p1, const Poly& p2)
{
    Poly poly = Poly();
    poly.addValuesFromPoly(p1);
    poly.substractValuesFromPoly(p2);
    return poly;
}

std::ostream& operator<<(std::ostream& output, const Poly& poly)
{
    int counter = 0;
    for (auto it = poly.values.rbegin(); it != poly.values.rend(); ++it) {
        if (it->second == 0) {
            continue;
        }
        counter++;
        if (it != poly.values.rbegin() && counter > 1) {
            if (it->second >= 0) {
                output << " + ";
            } else {
                output << " - ";
            }

            if (it->second != 1) {
                output << std::abs(it->second);
            }
        } else {

            if (it->second != 1) {
                output << it->second;
            }
        }

        if (it->first != 0) {
            output << "x^" << it->first;
        }
    }

    if (counter == 0) {
        output << 0;
    }

    output << std::endl;
    return output;
}
