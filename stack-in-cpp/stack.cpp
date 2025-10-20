#include "stack.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#define STACK_GROWTH_FACTOR 2

Stack::Stack(int initCap)
{
    this->top = 0;
    this->cap = initCap != 0 ? initCap : STACK_GROWTH_FACTOR;

    int* newPtr = (int*)malloc(cap * sizeof(int));
    if (newPtr == NULL) {
        errno = ENOMEM;
        return;
    }

    this->items = newPtr;
}

Stack::~Stack()
{
    free(this->items);
    this->items = NULL;
    this->top = 0;
    this->cap = 0;
}

Stack::Stack(const Stack& other)
{
    try {
        this->top = other.top;

        if (this->top > 0) {
            int* newPtr = (int*)malloc(this->top * sizeof(int));
            if (newPtr == NULL) {
                throw ENOMEM;
            }

            this->items = newPtr;
            this->cap = this->top;
            memcpy(newPtr, other.items, other.top * sizeof(int));

        } else {
            int* newPtr = (int*)malloc(STACK_GROWTH_FACTOR * sizeof(int));
            if (newPtr == NULL) {
                throw ENOMEM;
            }


            this->cap = STACK_GROWTH_FACTOR;
            this->items = newPtr;
        }
    } catch (...) {
        this->top = 0;
        this->cap = 0;
        if (this->items != NULL) {
            free(this->items);
        }
        this->items = NULL;
        errno = ENOMEM;
    }
}

Stack& Stack::operator=(const Stack& other)
{
    this->top = other.top;

    if (this->cap >= other.top) {
        memcpy(this->items, other.items, other.top * sizeof(int));
        return *this;
    }


    try {
        this->cap = other.top;
        int* newPtr = (int*)malloc(other.top * sizeof(int));
        if (newPtr == NULL) {
            throw ENOMEM;
        }
        if (this->items != NULL) {
            free(this->items);
            this->items = NULL;
        }
        this->items = newPtr;
        memcpy(this->items, other.items, other.top * sizeof(int));
    } catch (...) {
        this->top = 0;
        this->cap = 0;
        if (this->items != NULL) {
            free(this->items);
        }
        this->items = NULL;
        errno = ENOMEM;
    }
    return *this;
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
