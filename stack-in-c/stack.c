#include "stack.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 2

void init(Stack* s)
{
    s->top = 0;
    s->cap = INITIAL_CAPACITY;
    s->items = malloc(s->cap * sizeof(int));
    if (s->items == NULL) {
        errno = ENOMEM;
    }
}

void destroy(Stack* s)
{
    free(s->items);
    s->items = NULL;
    s->top = 0;
    s->cap = 0;
}

void push(Stack* s, int element)
{
    if (s->cap == 0) {
        errno = EPERM;
        return;
    }
    if (s->top >= s->cap) {
        // int newCap = (s->cap < 256) ? s->cap * 2 : s->cap + (s->cap + 768) / 4;
        if (s->cap > INT_MAX / 2) {
            errno = EOVERFLOW;
            return;
        }
        int newCap = s->cap * 2;
        void* newPtr = realloc(s->items, newCap * sizeof(int));

        if (newPtr == NULL) {
            errno = ENOMEM;
            return;
        }

        s->items = newPtr;
        s->cap = newCap;
    }
    s->items[s->top] = element;
    s->top++;
}

int pop(Stack* s)
{
    if (s->top > 0) {
        s->top--;
        return s->items[s->top];
    }
    errno = ENODATA;
    return -1;
}

bool isEmpty(const Stack* s)
{
    return s->top == 0;
}
