#pragma once

class Stack {
public:
    Stack(int initCap);
    ~Stack();

    Stack(const Stack& other);
    Stack& operator=(const Stack& other);

    void push(int element);
    int pop();
    void clear();
    bool isEmpty();

private:
    int top;
    int cap;
    int* items;
};
