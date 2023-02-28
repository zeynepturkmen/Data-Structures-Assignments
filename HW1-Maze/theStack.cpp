//  Created by Zeynep Türkmen on 23.03.2022.
#include <iostream>
#include "theStack.h"

using namespace std;

template <class T>
theStack<T>::theStack() //Constructs an empty stack
{
    top = NULL;
}

template <class T>
void theStack<T>::push(T val) //pushes the T object into the stack
{
    Node<T> *newNode;
    newNode = new Node<T>; //create a new node to store the value
    newNode->value = val;

    if (isEmpty()) //if its the first node
    {
        top = newNode; //make it the top node
        newNode->next = NULL;
    }
    else //insert it on the top of top
    {
        newNode->next = top;
        top = newNode;
    }
}

template <class T>
void theStack<T>::pop(T & val) //pops the top element and stores it into val
{
    Node<T> *temp;

    if (isEmpty())
    {
        cout << "The stack is empty.\n";
    }
    else //removes the value off top
    {
        val = top->value;
        temp = top->next;
        delete top;
        top = temp;
    }
}

template <class T>
bool theStack<T>::isEmpty() //returns true if the stack is empty, returns false otherwise
{
    bool check;
    if (top == NULL)
        check = true;
    else
        check = false;

    return check;
}
