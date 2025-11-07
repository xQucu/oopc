
#include "complex.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

void testConstructorsAndGetters()
{
    Complex a;
    assert(a.getReal() == 0);
    assert(a.getImag() == 0);

    Complex b(3, 4);
    assert(b.getReal() == 3);
    assert(b.getImag() == 4);

    Complex c(5.5);
    assert(c.getReal() == 5.5);
    assert(c.getImag() == 0);
}

void testSetters()
{
    Complex a;
    a.setReal(10);
    a.setImag(-2.5);
    assert(a.getReal() == 10);
    assert(a.getImag() == -2.5);
}

void testDivisionByZero()
{
    bool caught = false;

    try {
        Complex a(1, 2);
        Complex zero(0, 0);
        a / zero;
    } catch (...) {
        caught = true;
    }
    assert(caught);

    caught = false;


    try {
        Complex a(3, 4);
        a / 0.0;
    } catch (...) {
        caught = true;
    }
    assert(caught);

    caught = false;


    try {
        Complex zero(0, 0);
        5.0 / zero;
    } catch (...) {
        caught = true;
    }
    assert(caught);
}

void testArithmetic()
{
    Complex a(1, 2);
    Complex b(3, 4);

    Complex sum = a + b;
    assert(sum.getReal() == 4 && sum.getImag() == 6);

    Complex diff = b - a;
    assert(diff.getReal() == 2 && diff.getImag() == 2);

    Complex prod = a * b;
    assert(prod.getReal() == -5 && prod.getImag() == 10);

    Complex quot = b / a;
    double denom = 5;
    double expectedReal = (3 * 1 + 4 * 2) / denom; // 11 / 5 = 2.2
    double expectedImag = (4 * 1 - 3 * 2) / denom; // -2 / 5 = -0.4

    assert(quot.getReal() == expectedReal);
    assert(quot.getImag() == expectedImag);
}

void testCompoundAssignments()
{
    Complex a(1, 1), b(2, 3), c(4, -1);

    Complex b_copy = b;
    a += (b_copy += c);
    assert(a.getReal() == 7 && a.getImag() == 3);

    a = Complex(1, 1);
    b = Complex(2, 3);
    (a += b) += c;
    assert(a.getReal() == 7 && a.getImag() == 3);

    a = Complex(1, 2);
    b = Complex(3, 4);
    c = Complex(2, 1);
    a *= (b *= c);
    Complex expected_b(2, 11);
    assert(b.getReal() == expected_b.getReal());
    assert(b.getImag() == expected_b.getImag());

    a = Complex(1, 2);
    b = Complex(3, 4);
    c = Complex(2, 1);
    (a *= b) *= c;
    Complex expected_a((1 * 3 - 2 * 4), (1 * 4 + 2 * 3));
    expected_a = expected_a * c;
    assert(a.getReal() == expected_a.getReal());
    assert(a.getImag() == expected_a.getImag());
}

void testScalarArithmetic()
{
    Complex a(1, 2);
    Complex b(2, 3);

    Complex sum1 = a + 7;
    Complex sum2 = 7 + a;
    assert(sum1.getReal() == 8 && sum1.getImag() == 2);
    assert(sum2.getReal() == 8 && sum2.getImag() == 2);

    Complex prod1 = b * 5;
    Complex prod2 = 5 * b;
    assert(prod1.getReal() == 10 && prod1.getImag() == 15);
    assert(prod2.getReal() == 10 && prod2.getImag() == 15);
}

void testComparison()
{
    Complex c1(3.5, 0);
    Complex c2(3.5, 0);
    Complex c3(3.5, 1);

    assert(c1 == c2);
    assert(!(c1 != c2));
    assert(c1 != c3);
}

void testAmplitudePhase()
{
    Complex a(3, 4);
    assert(a.amplitude() == 5.0);
    assert(a.phase() == std::atan2(4, 3));
}

void testStream()
{
    Complex a(1, -2);
    std::ostringstream os;
    os << a;
    assert(os.str() == "(1,-2)");
}

void testChainedAndMixedOps()
{
    Complex a(1, 1), b(2, 3), c(4, -1);

    a += b += c;
    assert(a.getReal() == 7 && a.getImag() == 3);

    a = Complex(1, 1);
    b = Complex(2, 3);
    (a += b) += c;
    assert(a.getReal() == 7 && a.getImag() == 3);

    a = Complex(1, 2);
    b = Complex(3, 4);
    c = Complex(2, 1);
    a *= b *= c;
    Complex expected_b(2, 11);
    assert(b.getReal() == expected_b.getReal());
    assert(b.getImag() == expected_b.getImag());

    a = Complex(1, 2);
    b = Complex(3, 4);
    c = Complex(2, 1);
    (a *= b) *= c;
    Complex expected_a((1 * 3 - 2 * 4), (1 * 4 + 2 * 3));
    expected_a = expected_a * c;
    assert(a.getReal() == expected_a.getReal());
    assert(a.getImag() == expected_a.getImag());

    a = Complex(1, 2);
    Complex sum1 = a + 7;
    Complex sum2 = 7 + a;
    assert(sum1.getReal() == 8 && sum1.getImag() == 2);
    assert(sum2.getReal() == 8 && sum2.getImag() == 2);

    b = Complex(2, 3);
    Complex prod1 = b * 5;
    Complex prod2 = 5 * b;
    assert(prod1.getReal() == 10 && prod1.getImag() == 15);
    assert(prod2.getReal() == 10 && prod2.getImag() == 15);

    Complex c_num(3.5, 0);
    assert(c_num == Complex(3.5, 0));
    assert(Complex(3.5, 0) == c_num);
}

int main()
{
    testConstructorsAndGetters();
    testSetters();
    testArithmetic();
    testCompoundAssignments();
    testScalarArithmetic();
    testComparison();
    testAmplitudePhase();
    testStream();
    testChainedAndMixedOps();
    testDivisionByZero();

    std::cout << "All tests passed\n";
    return 0;
}
