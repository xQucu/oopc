// Class Poly is defined in file Poly.h
#include "poly.h"

#include <iostream>

using namespace std;

template <typename T>
void display(T& p);
void test(const Poly& p1, const Poly& p2);
void testAddition(const Poly& p1, const Poly& p2);
void testSubtraction(const Poly& p1, const Poly& p2);
void testMultiplication(const Poly& p1, const Poly& p2);
void testValue(const Poly& p1, const Poly& p2);

int main()
{
    // Declare object representing polynomial p1
    Poly p1;

    // Specify coefficients of p1 = -2.3x^3 + 5x^2 - 4.5
    // p1[3] = -2.3;
    // p1[2] = 5;
    // p1[0] = -4.5;
    //
    // cout << "======= Polynomial p1 =======" << endl;
    // cout << p1 << endl;
    //
    // // Declare object representing polynomial p2 = 2.6
    // Poly p2 = 2.6;
    //
    // // Specify additional coefficient of p2 = x^4 - 7x^2 + 3x + 2.6
    // p2[1] = 3;
    // p2[2] = -7;
    // p2[4] = 1;
    //
    // cout << "\n======= Polynomial p2 =======" << endl;
    // display(p2);
    // display<const Poly>(p2);
    //
    // test(p1, p2);
}

template <typename T>
void display(T& p)
{
    cout << p << endl;

    // Display coefficients. If coefficient does not exist, display 0.
    for (int i = 5; i >= 0; --i) {
        cout << "\tCoefficient " << i << ": " << p[i] << endl;
    }
}

void test(const Poly& p1, const Poly& p2)
{
    testAddition(p1, p2);
    testSubtraction(p1, p2);
    testMultiplication(p1, p2);
    testValue(p1, p2);
}

void testAddition(const Poly& p1, const Poly& p2)
{
    cout << "\n======= Addition =======" << endl;

    Poly p = p1 + p2;
    cout << "Sum of polynomials p1 and p2: " << p << endl;

    p = p1 + 2.4;
    cout << "Sum of polynomial p1 and 2.4: " << p << endl;

    p = 2.4 + p1;
    cout << "Sum of 2.4 and polynomial p1: " << p << endl;
}

void testSubtraction(const Poly& p1, const Poly& p2)
{
    cout << "\n======= Subtraction =======" << endl;

    cout << "Difference of polynomials p1 and p2: " << p1 - p2 << endl;

    // The following should display 0 (not 0x^3 + 0x^2 + 0)
    cout << "Difference of polynomials p1 and p1: " << p1 - p1 << endl;

    cout << "Difference of polynomial p1 and 5.3: " << p1 - 5.3 << endl;
    cout << "Difference of 5.3 and polynomial p1: " << 5.3 - p1 << endl;
}

void testMultiplication(const Poly& p1, const Poly& p2)
{
    cout << "\n======= Multiplication =======" << endl;

    cout << "Product of polynomials p1 and p2: " << p1 * p2 << endl;
    cout << "6.7 multiplied by polynomial p1: " << 6.7 * p1 << endl;
    cout << "Polynomial p1 multiplied by 6.7: " << p1 * 6.7 << endl;
}

void testValue(const Poly& p1, const Poly& p2)
{
    cout << "\n======= Value =======" << endl;

    cout << "Value of polynomial p1 at point  3.9: " << p1(3.9) << endl;
    cout << "Value of polynomial p2 at point -2.8: " << p2(-2.8) << endl;
}
