#pragma once

class Stack {
public:
    Stack(int initCap);
    ~Stack();

    void push(int element);
    int pop();
    void clear();
    bool isEmpty();

private:
    int top;
    int cap;
    int* items;
};
