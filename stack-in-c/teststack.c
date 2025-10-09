#include "stack.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>

int main()
{
    Stack s1;
    Stack s2;
    Stack s3;

    init(&s1);
    init(&s2);
    init(&s3);

    push(&s1, 1);
    push(&s1, 2);
    push(&s1, 3);
    push(&s2, 5);
    push(&s2, 6);

    assert(pop(&s1) == 3);
    assert(pop(&s1) == 2);

    assert(!isEmpty(&s1));

    assert(pop(&s1) == 1);

    assert(isEmpty(&s1));

    assert(pop(&s2) == 6);
    assert(pop(&s2) == 5);
    assert(pop(&s2) == -1);
    assert(errno == ENODATA);

    destroy(&s1);
    destroy(&s2);

    assert(pop(&s2) == -1);
    assert(errno == ENODATA);
    push(&s1, 4);
    assert(errno == EPERM);

    for (int i = 0; i <= 10000; i++) {
        push(&s3, i);
    }

    for (int i = 10000; i >= 0; i--) {
        assert(pop(&s3) == i);
    }

    destroy(&s3);

    return 0;
}
