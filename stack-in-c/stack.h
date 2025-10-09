#ifndef STACK_H
#define STACK_H


#include <stdbool.h>

typedef struct {
    int* items;
    int top;
    int cap;
} Stack;

void init(Stack* s);
void destroy(Stack* s);
void push(Stack* s, int element);
int pop(Stack* s);
bool isEmpty(const Stack* s);

#endif
