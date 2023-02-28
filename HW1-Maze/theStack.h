//Created by Zeynep Turkmen on 22.03.2022.
#ifndef theStack_h
#define theStack_h

template <class T>
struct Node //node for the stack object
{
    T value; //stored object
    Node<T> *next; //pointer to the next object
};

template <class T>
class theStack
{
    private:
        Node<T> *top; //top of the stack

    public:
        theStack(); //default constructor
        void push(T); //pushes T object into the stack
        void pop(T &); //pops the value on top of the stack and stores it into the inputted value
        bool isEmpty(); //checks if the stack is empty
};
#include "theStack.cpp"
#endif
