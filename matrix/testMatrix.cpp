#include "matrix.h"
#include <cassert>
#include <fstream>

using namespace std;

void testBasicConstruction()
{
    Matrix m(3, 3);
    assert(m.getRefCount() == 1);
    assert(m.getRows() == 3);
    assert(m.getCols() == 3);
    assert(m(0, 0) == 0);
    assert(m(2, 2) == 0);
}

void testCopyConstructor()
{
    Matrix m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    assert(m1.getRefCount() == 1);

    Matrix m2(m1);
    assert(m1.getRefCount() == 2);
    assert(m2.getRefCount() == 2);
    assert(m2(0, 0) == 1);
    assert(m2(0, 1) == 2);
    assert(m2(1, 0) == 3);
    assert(m2(1, 1) == 4);
}

void testAssignmentOperator()
{
    Matrix m1(2, 2);
    m1(0, 0) = 5;
    m1(1, 1) = 6;

    Matrix m2(3, 3);
    assert(m1.getRefCount() == 1);
    assert(m2.getRefCount() == 1);

    m2 = m1;
    assert(m1.getRefCount() == 2);
    assert(m2.getRefCount() == 2);
    assert(m2.getRows() == 2);
    assert(m2.getCols() == 2);
    assert(m2(0, 0) == 5);
    assert(m2(1, 1) == 6);
}

void testCopyOnWrite()
{
    Matrix m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    Matrix m2(m1);
    assert(m1.getRefCount() == 2);

    int val = m2(0, 0);
    assert(val == 1);
    assert(m1.getRefCount() == 2);

    m2(0, 0) = 99;
    assert(m1.getRefCount() == 1);
    assert(m2.getRefCount() == 1);
    assert(m1(0, 0) == 1);
    assert(m2(0, 0) == 99);
}

void testArithmeticOperators()
{
    Matrix m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    Matrix m2(2, 2);
    m2(0, 0) = 5;
    m2(0, 1) = 6;
    m2(1, 0) = 7;
    m2(1, 1) = 8;

    Matrix m3 = m1 + m2;
    assert(m3(0, 0) == 6);
    assert(m3(0, 1) == 8);
    assert(m3(1, 0) == 10);
    assert(m3(1, 1) == 12);

    Matrix m4 = m1 - m2;
    assert(m4(0, 0) == -4);
    assert(m4(0, 1) == -4);
    assert(m4(1, 0) == -4);
    assert(m4(1, 1) == -4);

    Matrix m5 = m1 * m2;
    assert(m5(0, 0) == 19);
    assert(m5(0, 1) == 22);
    assert(m5(1, 0) == 43);
    assert(m5(1, 1) == 50);

    Matrix m6(2, 2);
    m6(0, 0) = 1;
    m6(0, 1) = 2;
    m6(1, 0) = 3;
    m6(1, 1) = 4;

    m6 += m2;
    assert(m6(0, 0) == 6);
    assert(m6(0, 1) == 8);
    assert(m6(1, 0) == 10);
    assert(m6(1, 1) == 12);

    Matrix m7(2, 2);
    m7(0, 0) = 10;
    m7(0, 1) = 10;
    m7(1, 0) = 10;
    m7(1, 1) = 10;

    m7 -= m2;
    assert(m7(0, 0) == 5);
    assert(m7(0, 1) == 4);
    assert(m7(1, 0) == 3);
    assert(m7(1, 1) == 2);

    Matrix m8(2, 2);
    m8(0, 0) = 1;
    m8(0, 1) = 2;
    m8(1, 0) = 3;
    m8(1, 1) = 4;

    m8 *= m2;
    assert(m8(0, 0) == 19);
    assert(m8(0, 1) == 22);
    assert(m8(1, 0) == 43);
    assert(m8(1, 1) == 50);
}

void testComparisonOperators()
{
    Matrix m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    Matrix m2(2, 2);
    m2(0, 0) = 1;
    m2(0, 1) = 2;
    m2(1, 0) = 3;
    m2(1, 1) = 4;

    Matrix m3(2, 2);
    m3(0, 0) = 5;
    m3(0, 1) = 6;
    m3(1, 0) = 7;
    m3(1, 1) = 8;

    assert(m1 == m2);
    assert(!(m1 != m2));
    assert(m1 != m3);
    assert(!(m1 == m3));
}

void testStreamInput()
{
    ifstream inFile("test_matrix.txt");
    assert(inFile.is_open());

    Matrix m(inFile);
    inFile.close();

    assert(m.getRows() == 3);
    assert(m.getCols() == 3);
    assert(m(0, 0) == 1);
    assert(m(0, 1) == 2);
    assert(m(0, 2) == 3);
    assert(m(1, 0) == 4);
    assert(m(1, 1) == 5);
    assert(m(1, 2) == 6);
    assert(m(2, 0) == 7);
    assert(m(2, 1) == 8);
    assert(m(2, 2) == 9);
}

void testExceptions()
{
    bool exceptionThrown = false;

    try {
        Matrix m(-1, 5);
    } catch (const DimensionMismatchException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        Matrix m(3, 3);
        m(5, 5);
    } catch (const IndexOutOfBoundsException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        Matrix m1(2, 2);
        Matrix m2(3, 3);
        Matrix m3 = m1 + m2;
    } catch (const DimensionMismatchException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        Matrix m1(2, 3);
        Matrix m2(2, 2);
        Matrix m3 = m1 * m2;
    } catch (const DimensionMismatchException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        Matrix m1(2, 2);
        Matrix m2(3, 3);
        Matrix m3 = m1 - m2;
    } catch (const DimensionMismatchException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        ifstream inFile("nonexistent_file.txt");
        Matrix m(inFile);
    } catch (const FileReadException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
}

void testMultipleReferences()
{
    Matrix m1(2, 2);
    m1(0, 0) = 10;

    Matrix m2 = m1;
    Matrix m3 = m2;
    Matrix m4 = m3;

    assert(m1.getRefCount() == 4);

    m4(0, 0) = 20;
    assert(m1.getRefCount() == 3);
    assert(m4.getRefCount() == 1);
    assert(m1(0, 0) == 10);
    assert(m4(0, 0) == 20);
}

void testConstAccess()
{
    Matrix m1(2, 2);

    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    Matrix m2(m1);
    assert(m1.getRefCount() == 2);
    assert(m2.getRefCount() == 2);

    const Matrix& constRef = m2;
    int val = constRef(0, 0);
    assert(val == 1);
    assert(m1.getRefCount() == 2);
    assert(m2.getRefCount() == 2);

    constRef(0, 1);
    constRef(1, 0);
    constRef(1, 1);
    assert(m1.getRefCount() == 2);
}

void testReadVsWriteAccess()
{
    Matrix m1(2, 2);
    m1(0, 0) = 1;
    m1(1, 1) = 2;

    Matrix m2 = m1;
    assert(m1.getRefCount() == 2);

    m2(0, 0);
    m2(0, 1);
    m2(1, 0);
    m2(1, 1);

    assert(m1.getRefCount() == 2);

    m2(0, 0) = 100;
    assert(m1.getRefCount() == 1);
    assert(m2.getRefCount() == 1);
    assert(m1(0, 0) == 1);
    assert(m2(0, 0) == 100);
}

int main()
{
    testBasicConstruction();
    testCopyConstructor();
    testAssignmentOperator();
    testCopyOnWrite();
    testReadVsWriteAccess();
    testArithmeticOperators();
    testComparisonOperators();
    testStreamInput();
    testExceptions();
    testMultipleReferences();
    testConstAccess();

    return 0;
}
