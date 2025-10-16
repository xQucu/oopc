#include "stack.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#define STACK_GROWTH_FACTOR 2

Stack::Stack(int initCap)
{
    this->top = 0;
    this->cap = initCap;

    int* newItems = (int*)malloc(cap * sizeof(int));
    if (newItems == NULL) {
        errno = ENOMEM;
        return;
    }

    this->items = newItems;
}

Stack::~Stack()
{
    free(this->items);
    this->items = NULL;
    this->top = 0;
    this->cap = 0;
}

void Stack::push(int element)
{
    if (this->cap == 0) {
        errno = EPERM;
        return;
    }

    if (this->top >= this->cap) {
        int newCap = this->cap * STACK_GROWTH_FACTOR;
        void* newPtr = realloc(this->items, newCap * sizeof(int));

        if (newPtr == NULL) {
            errno = ENOMEM;
            return;
        }

        this->items = (int*)newPtr;
        this->cap = newCap;
    }
    this->items[this->top] = element;
    this->top++;
}

int Stack::pop()
{

    if (this->top > 0) {
        this->top--;
        return this->items[this->top];
    }
    errno = ENODATA;
    return -1;
}

bool Stack::isEmpty()
{
    return this->top == 0;
}
