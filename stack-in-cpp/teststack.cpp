#include "stack.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>


void testBasicPushPop();
void testIsEmpty();
void testPopErrorCases();
void stackCapGrowthTest(int numberOfElements);

int main()
{
    testBasicPushPop();
    testIsEmpty();
    testPopErrorCases();

    int numberOfElements = 10000;
    stackCapGrowthTest(numberOfElements);

    return 0;
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
