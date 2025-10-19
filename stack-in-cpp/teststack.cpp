#include "stack.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>


void testBasicPushPop();
void testIsEmpty();
void testPopErrorCases();
void stackCapGrowthTest(int numberOfElements);
void testCopy();
void testAssignment();
void testPassingObject();

int main()
{
    testBasicPushPop();
    testIsEmpty();
    testPopErrorCases();

    int numberOfElements = 10000;
    stackCapGrowthTest(numberOfElements);

    return 0;
}

void testCopy()
{
    Stack s1(3);
    s1.push(1);
    s1.push(2);
    s1.push(3);

    Stack s2(s1);

    assert(s1.pop() == s2.pop());
    s2.push(4);
    assert(s1.pop() == 3);
    assert(s2.pop() == 4);

    Stack s3(0);
    Stack s4(s3);

    s4.push(6);
    assert(s4.pop() == 6);
}

void testAssignment()
{
    Stack s1(3);
    s1.push(1);
    s1.push(2);
    s1.push(3);

    Stack s2(4);
    s2.push(10);
    s2.push(20);
    s2.push(30);
    s2.push(40);

    s2 = s1;
    assert(s1.pop() == s2.pop());

    s2 = s1;
    assert(s1.pop() == s2.pop());

    s2.pop();

    s1 = s2;
    assert(s1.pop() == s2.pop());

    s1.push(5);
    s1.push(8);
    s1.push(25);
    Stack s3(0);
    s1 = s3;
    assert(s1.isEmpty());

    s2.push(10);
    s2.push(20);
    s2.push(30);
    s2.push(40);
    s1 = s2;
    assert(s1.pop() == 40);
    assert(s2.pop() == 40);

    s3 = s3;
    assert(s3.isEmpty());
}

void stackCapGrowthTest(int numberOfElements)
{
    Stack s(2);

    for (int i = 0; i <= numberOfElements; i++) {
        s.push(i);
    }

    for (int i = numberOfElements; i >= 0; i--) {
        assert(s.pop() == i);
    }
}

void testPopErrorCases()
{
    Stack s(2);
    assert(s.pop() == -1);
    assert(errno == ENODATA);

    errno = 0;

    assert(s.pop() == -1);
    assert(errno == ENODATA);
}

void testIsEmpty()
{
    Stack s1(2);
    s1.push(5);
    s1.pop();
    assert(s1.isEmpty());

    Stack s2(2);
    s2.push(5);
    assert(!s2.isEmpty());
}

void testBasicPushPop()
{
    Stack s1(2);
    Stack s2(2);
    Stack s3(2);
    Stack s4(2);

    s1.push(5);
    s1.push(6);
    s1.pop();
    s1.pop();
    s2.push(5);
    s2.push(6);
    s2.pop();
    s2.pop();
    s3.push(5);
    s4.push(6);
    s3.push(6);
    s3.pop();
    s3.pop();
    s4.push(5);
    s4.pop();
    s4.pop();
}
