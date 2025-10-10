#include "stack.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include "teststack.h"

int main()
{
    testBasicPushPop();
    testIsEmpty();
    testPopErrorCases();
    testPushingAfterDestroing();

    int numberOfElements = 10000;
    stackCapGrowthTest(numberOfElements);

    return 0;
}

void testPushingAfterDestroing()
{
    Stack s5;
    init(&s5);
    destroy(&s5);
    push(&s5, 5);
    assert(errno == EPERM);
}

void stackCapGrowthTest(int numberOfElements)
{
    Stack s3;
    init(&s3);

    for (int i = 0; i <= numberOfElements; i++)
    {
        push(&s3, i);
    }

    for (int i = numberOfElements; i >= 0; i--)
    {
        assert(pop(&s3) == i);
    }

    destroy(&s3);
}

void testPopErrorCases()
{
    Stack s6;
    init(&s6);
    assert(pop(&s6) == -1);
    assert(errno == ENODATA);

    errno = 0;

    destroy(&s6);
    assert(pop(&s6) == -1);
    assert(errno == ENODATA);
}

void testIsEmpty()
{
    Stack s5;
    init(&s5);
    push(&s5, 5);
    pop(&s5);
    assert(isEmpty(&s5));
    destroy(&s5);

    Stack s6;
    init(&s6);
    push(&s6, 5);
    assert(!isEmpty(&s6));
    destroy(&s6);
}
void testBasicPushPop()
{
    Stack s1;
    Stack s2;
    Stack s3;
    Stack s4;

    init(&s1);
    init(&s2);
    init(&s3);
    init(&s4);

    push(&s1, 5);
    push(&s1, 6);
    pop(&s1);
    pop(&s1);
    push(&s2, 5);
    push(&s2, 6);
    pop(&s2);
    pop(&s2);
    push(&s3, 5);
    push(&s4, 6);
    push(&s3, 6);
    pop(&s3);
    pop(&s3);
    push(&s4, 5);
    pop(&s4);
    pop(&s4);

    destroy(&s1);
    destroy(&s2);
    destroy(&s3);
    destroy(&s4);
}
