#pragma once

#include <map>
#include <ostream>

class Poly {
public:
    Poly(double val = 0);

    double& operator[](int key);
    double operator[](int key) const;
    friend std::ostream& operator<<(std::ostream& output, const Poly& toOutput);
    friend Poly operator+(const Poly& p1, const Poly& p2);
    void addValuesFromPoly(const Poly& p1);
    friend Poly operator-(const Poly& p1, const Poly& p2);
    void substractValuesFromPoly(const Poly& p);

private:
    std::map<int, double> values;
};
