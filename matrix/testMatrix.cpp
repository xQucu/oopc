#include "matrix.h"
#include <fstream>
#include <iostream>

using namespace std;

void testBasicConstruction()
{
    cout << "=== Test: Basic Construction ===" << endl;
    try {
        Matrix m(3, 3);
        cout << "Created 3x3 matrix (refCount: " << m.getRefCount() << ")" << endl;
        cout << m << endl;
        cout << "PASSED" << endl
             << endl;
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testCopyConstructor()
{
    cout << "=== Test: Copy Constructor & Reference Counting ===" << endl;
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(1, 0) = 3;
        m1(1, 1) = 4;

        cout << "Original matrix m1 (refCount: " << m1.getRefCount() << "):" << endl;
        cout << m1 << endl
             << endl;

        Matrix m2(m1);
        cout << "After copy construction m2(m1):" << endl;
        cout << "m1 refCount: " << m1.getRefCount() << endl;
        cout << "m2 refCount: " << m2.getRefCount() << endl;
        cout << "m2 content:" << endl
             << m2 << endl;

        if (m1.getRefCount() == 2 && m2.getRefCount() == 2) {
            cout << "PASSED: Reference counting works correctly" << endl
                 << endl;
        } else {
            cout << "FAILED: Reference counting incorrect" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testAssignmentOperator()
{
    cout << "=== Test: Assignment Operator ===" << endl;
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 5;
        m1(1, 1) = 6;

        Matrix m2(3, 3);
        cout << "Before assignment:" << endl;
        cout << "m1 refCount: " << m1.getRefCount() << endl;
        cout << "m2 refCount: " << m2.getRefCount() << endl;

        m2 = m1;
        cout << "After m2 = m1:" << endl;
        cout << "m1 refCount: " << m1.getRefCount() << endl;
        cout << "m2 refCount: " << m2.getRefCount() << endl;
        cout << "m2 content:" << endl
             << m2 << endl;

        if (m1.getRefCount() == 2 && m2.getRefCount() == 2) {
            cout << "PASSED" << endl
                 << endl;
        } else {
            cout << "FAILED" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testCopyOnWrite()
{
    cout << "=== Test: Copy-On-Write Mechanism ===" << endl;
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(1, 0) = 3;
        m1(1, 1) = 4;

        Matrix m2(m1);
        cout << "After m2(m1), refCount: " << m1.getRefCount() << endl;

        // Reading from m2 should NOT trigger copy
        int val = m2(0, 0);
        cout << "After reading m2(0,0) = " << val << ", refCount: " << m1.getRefCount() << endl;

        // Writing to m2 should trigger copy
        m2(0, 0) = 99;
        cout << "After writing m2(0,0) = 99:" << endl;
        cout << "m1 refCount: " << m1.getRefCount() << endl;
        cout << "m2 refCount: " << m2.getRefCount() << endl;
        cout << "m1(0,0) = " << m1(0, 0) << " (should be 1)" << endl;
        cout << "m2(0,0) = " << m2(0, 0) << " (should be 99)" << endl;

        if (m1.getRefCount() == 1 && m2.getRefCount() == 1 && m1(0, 0) == 1 && m2(0, 0) == 99) {
            cout << "PASSED: Copy-on-write works correctly" << endl
                 << endl;
        } else {
            cout << "FAILED: Copy-on-write not working properly" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testArithmeticOperators()
{
    cout << "=== Test: Arithmetic Operators ===" << endl;
    try {
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

        cout << "m1:" << endl
             << m1 << endl
             << endl;
        cout << "m2:" << endl
             << m2 << endl
             << endl;

        Matrix m3 = m1 + m2;
        cout << "m1 + m2:" << endl
             << m3 << endl
             << endl;

        Matrix m4 = m1 - m2;
        cout << "m1 - m2:" << endl
             << m4 << endl
             << endl;

        Matrix m5 = m1 * m2;
        cout << "m1 * m2:" << endl
             << m5 << endl
             << endl;

        m1 += m2;
        cout << "After m1 += m2:" << endl
             << m1 << endl
             << endl;

        cout << "PASSED" << endl
             << endl;
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testComparisonOperators()
{
    cout << "=== Test: Comparison Operators ===" << endl;
    try {
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

        cout << "m1 == m2: " << (m1 == m2 ? "true" : "false") << " (should be true)" << endl;
        cout << "m1 != m3: " << (m1 != m3 ? "true" : "false") << " (should be true)" << endl;

        if (m1 == m2 && m1 != m3) {
            cout << "PASSED" << endl
                 << endl;
        } else {
            cout << "FAILED" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testStreamInput()
{
    cout << "=== Test: Stream Input from File ===" << endl;
    try {
        // Create a test file
        ofstream outFile("test_matrix.txt");
        outFile << "3 3\n";
        outFile << "1 2 3\n";
        outFile << "4 5 6\n";
        outFile << "7 8 9\n";
        outFile.close();

        // Read from file using ifstream
        ifstream inFile("test_matrix.txt");
        if (!inFile.is_open()) {
            throw FileReadException("Cannot open test file");
        }

        Matrix m(inFile);
        inFile.close();

        cout << "Matrix read from file stream:" << endl;
        cout << m << endl;

        if (m.getRows() == 3 && m.getCols() == 3 && m(0, 0) == 1 && m(2, 2) == 9) {
            cout << "PASSED" << endl
                 << endl;
        } else {
            cout << "FAILED: Wrong values" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testExceptions()
{
    cout << "=== Test: Exception Handling ===" << endl;

    // Test 1: Invalid dimensions
    cout << "Test 1: Invalid dimensions" << endl;
    try {
        Matrix m(-1, 5);
        cout << "FAILED: Should have thrown InvalidDimensionException" << endl;
    } catch (const InvalidDimensionException& e) {
        cout << "PASSED: Caught InvalidDimensionException: " << e.what() << endl;
    } catch (...) {
        cout << "FAILED: Wrong exception type" << endl;
    }

    // Test 2: Index out of bounds
    cout << "Test 2: Index out of bounds" << endl;
    try {
        Matrix m(3, 3);
        int val = m(5, 5);
        cout << "FAILED: Should have thrown IndexOutOfBoundsException" << endl;
    } catch (const IndexOutOfBoundsException& e) {
        cout << "PASSED: Caught IndexOutOfBoundsException: " << e.what() << endl;
    } catch (...) {
        cout << "FAILED: Wrong exception type" << endl;
    }

    // Test 3: Dimension mismatch in addition
    cout << "Test 3: Dimension mismatch in addition" << endl;
    try {
        Matrix m1(2, 2);
        Matrix m2(3, 3);
        Matrix m3 = m1 + m2;
        cout << "FAILED: Should have thrown DimensionMismatchException" << endl;
    } catch (const DimensionMismatchException& e) {
        cout << "PASSED: Caught DimensionMismatchException: " << e.what() << endl;
    } catch (...) {
        cout << "FAILED: Wrong exception type" << endl;
    }

    // Test 4: Invalid matrix multiplication dimensions
    cout << "Test 4: Invalid matrix multiplication dimensions" << endl;
    try {
        Matrix m1(2, 3);
        Matrix m2(2, 2);
        Matrix m3 = m1 * m2;
        cout << "FAILED: Should have thrown DimensionMismatchException" << endl;
    } catch (const DimensionMismatchException& e) {
        cout << "PASSED: Caught DimensionMismatchException: " << e.what() << endl;
    } catch (...) {
        cout << "FAILED: Wrong exception type" << endl;
    }

    cout << endl;
}

void testMultipleReferences()
{
    cout << "=== Test: Multiple References ===" << endl;
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 10;

        Matrix m2 = m1;
        Matrix m3 = m2;
        Matrix m4 = m3;

        cout << "After creating m2, m3, m4 from m1:" << endl;
        cout << "m1 refCount: " << m1.getRefCount() << " (should be 4)" << endl;

        m4(0, 0) = 20; // Should trigger copy
        cout << "After modifying m4:" << endl;
        cout << "m1 refCount: " << m1.getRefCount() << " (should be 3)" << endl;
        cout << "m4 refCount: " << m4.getRefCount() << " (should be 1)" << endl;
        cout << "m1(0,0): " << m1(0, 0) << " (should be 10)" << endl;
        cout << "m4(0,0): " << m4(0, 0) << " (should be 20)" << endl;

        if (m1.getRefCount() == 3 && m4.getRefCount() == 1) {
            cout << "PASSED" << endl
                 << endl;
        } else {
            cout << "FAILED" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testConstAccess()
{
    cout << "=== Test: Const Access (Read-Only) ===" << endl;
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(1, 0) = 3;
        m1(1, 1) = 4;

        const Matrix& m2 = m1;

        cout << "m1 refCount before const access: " << m1.getRefCount() << endl;

        int val = m2(0, 0); // Should use const version, no copy
        cout << "Read value from const reference: " << val << endl;
        cout << "m1 refCount after const access: " << m1.getRefCount() << " (should still be 1)" << endl;

        if (m1.getRefCount() == 1) {
            cout << "PASSED: Const access doesn't trigger copy" << endl
                 << endl;
        } else {
            cout << "FAILED" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

void testReadVsWriteAccess()
{
    cout << "=== Test: Read vs Write Access Distinction ===" << endl;
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 1;
        m1(1, 1) = 2;

        Matrix m2 = m1;
        cout << "After m2 = m1, refCount: " << m1.getRefCount() << endl;

        // Multiple reads should not trigger copy
        int v1 = m2(0, 0);
        int v2 = m2(0, 1);
        int v3 = m2(1, 0);
        int v4 = m2(1, 1);

        cout << "After 4 reads from m2, refCount: " << m1.getRefCount() << " (should be 2)" << endl;

        // First write triggers copy
        m2(0, 0) = 100;
        cout << "After 1 write to m2, m1 refCount: " << m1.getRefCount() << " (should be 1)" << endl;
        cout << "After 1 write to m2, m2 refCount: " << m2.getRefCount() << " (should be 1)" << endl;

        if (m1.getRefCount() == 1 && m2.getRefCount() == 1) {
            cout << "PASSED: Read operations don't copy, write does" << endl
                 << endl;
        } else {
            cout << "FAILED" << endl
                 << endl;
        }
    } catch (const MatrixException& e) {
        cout << "FAILED: " << e.what() << endl
             << endl;
    }
}

int main()
{
    cout << "========================================" << endl;
    cout << "   MATRIX CLASS COMPREHENSIVE TESTS   " << endl;
    cout << "========================================" << endl
         << endl;

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

    cout << "========================================" << endl;
    cout << "         ALL TESTS COMPLETED           " << endl;
    cout << "========================================" << endl;

    return 0;
}
