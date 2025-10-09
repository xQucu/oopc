#include "stack.h"
#include <errno.h>
// #include <stdbool.h>
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
    s->top = 0;
    s->cap = 0;
    free(s->items);
    s->items = NULL;
}

void push(Stack* s, int element)
{
    if (s->top >= s->cap) {
        int newCap = (s->cap < 256) ? s->cap * 2 : (s->cap + 768) / 4;
        void* newPtr = realloc(s->items, newCap);

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
    return 0;
}

bool isEmpty(const Stack* s)
{
    if (s->top == 0) {
        return true;
    }
    return false;
}
