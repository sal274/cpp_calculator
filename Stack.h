/*
Stack.h

Stack.h declares class template Stack

Class template Stack represents a stack data structure
*/

#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include <iostream>

template <class T>
class Stack
{
    public:

        Stack();
        ~Stack();

        Stack(const Stack& source);
        Stack(Stack&& source);

        Stack& operator=(const Stack& source);
        Stack& operator=(Stack&& source);

        bool empty() const { return sz == 0; }
        std::size_t size() const { return sz; }

        T& top();
        const T& top() const;

        void push(const T& val);
        void push(T&& val);

        void pop();

        void clear();

        void print() const;  // For debugging

    private:

        struct Node;

        Node* head;
        Node* tail;
        std::size_t sz;
};

template <class T>
struct Stack<T>::Node
{
    Node* next;
    Node* prev;
    T val;

    Node(const T& v = T{}) : val{ v }, next{ nullptr }, prev{ nullptr } {}
    Node(T&& v) : val{ std::move(v) }, next{ nullptr }, prev{ nullptr } {}
};

template <class T>
Stack<T>::Stack() : head{ nullptr }, tail{ nullptr }, sz{ 0 }
{}

template <class T>
Stack<T>::~Stack()
{
    Node* temp = head;
    while (temp)
    {
        head = head -> next;
        delete temp;
        temp = head;
    }
}

template <class T>
Stack<T>::Stack(const Stack& source) : head{ nullptr }, tail{ nullptr },
        sz{ 0 }
{
    Node* trav = source.head;
    while (trav)
    {
        push(trav -> val);
        trav = trav -> next;
    }
}

template <class T>
Stack<T>::Stack(Stack&& source) : head{ source.head }, tail{ source.tail },
        sz{ source.sz }
{
    source.head = nullptr;
    source.tail = nullptr;
    sz = 0;
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack& source)
{
    Stack copy = source;
    std::swap(*this, copy);
    return *this;
}

template <class T>
Stack<T>& Stack<T>::operator=(Stack&& source)
{
    std::swap(head, source.head);
    std::swap(tail, source.tail);
    std::swap(sz, source.sz);

    return *this;
}

template <class T>
T& Stack<T>::top()
{
    if (!head) throw std::out_of_range("top(): stack is empty");
    return tail -> val;
}

template <class T>
const T& Stack<T>::top() const
{
    if (!head) throw std::out_of_range("top(): stack is empty");
    return tail -> val;
}

template <class T>
void Stack<T>::push(const T& val)
{
    Node* newnode = new Node(val);
    if (!head)
    {
        // This is the first node
        head = newnode;
        tail = newnode;
    }
    else
    {
        // Push at end
        newnode -> prev = tail;
        tail -> next = newnode;
        tail = newnode;
    }
    ++sz;
}

template <class T>
void Stack<T>::push(T&& val)
{
    Node* newnode = new Node(std::move(val));
    if (!head)
    {
        // This is the first node
        head = newnode;
        tail = newnode;
    }
    else
    {
        // Push at end
        newnode -> prev = tail;
        tail -> next = newnode;
        tail = newnode;
    }
    ++sz;
}

template <class T>
void Stack<T>::pop()
{
    if (!head) throw std::out_of_range("pop(): can not pop from emtpy stack");

    if (head == tail)
    {
        // There is only one node
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        // Delete node at the end (sz > 1)
        Node* temp = tail;
        tail = tail -> prev;
        tail -> next = nullptr;
        delete temp;
    }
    --sz;
}

template <class T>
void Stack<T>::clear()
{
    Node* temp = head;
    while (temp)
    {
        head = head -> next;
        delete temp;
        temp = head;
    }

    head = nullptr;
    tail = nullptr;
    sz = 0;
}

// For debugging
template <class T>
void Stack<T>::print() const
{
    if (!head)
    {
        std::cout << "[]\n";
        return;
    }
    Node* trav = head;
    std::cout << "[";
    while (trav)
    {
        if (trav == tail) std::cout << trav -> val << "]\n";
        else std::cout << trav -> val << ", ";
        trav = trav -> next;
    }
}

#endif