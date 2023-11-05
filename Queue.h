/*
Queue.h

Queue.h declares class template Queue

Class template Queue represents a queue data structure
*/

/*
DSA Assignment 2

Salman Abdullah
221478 BSCS IIIC

11 October 2023
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include <iostream>

template <class T>
class Queue
{
    public:

        Queue();
        ~Queue();

        Queue(const Queue& source);
        Queue(Queue&& source);

        Queue& operator=(const Queue& source);
        Queue& operator=(Queue&& source);

        bool empty() const { return sz == 0; }
        std::size_t size() const { return sz; }

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        void enqueue(const T& val);
        void enqueue(T&& val);

        void dequeue();

        void clear();

        void print() const;  // For debugging

    private:

        struct Node;

        Node* head;
        Node* tail;

        std::size_t sz;
};

template <class T>
struct Queue<T>::Node
{
    Node* next;
    Node* prev;

    T val;

    Node(const T& v = T{}) : val{ v }, next{ nullptr }, prev{ nullptr } {}
    Node(T&& v) : val{ v }, next{ nullptr }, prev{ nullptr } {}
};

template <class T>
Queue<T>::Queue() : head{ nullptr }, tail{ nullptr }, sz{ 0 }
{}

template <class T>
Queue<T>::~Queue()
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
Queue<T>::Queue(const Queue& source) : head{ nullptr }, tail{ nullptr },
        sz{ 0 }
{
    Node* trav = source.head;
    while (trav)
    {
        enqueue(trav -> val);
        trav = trav -> next;
    }
}

template <class T>
Queue<T>::Queue(Queue&& source) : head{ source.head }, tail{ source.tail },
        sz{ source.sz }
{
    source.head = nullptr;
    source.tail = nullptr;
    source.sz = 0;
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue& source)
{
    Queue copy = source;
    std::swap(*this, copy);
    return *this;
}

template <class T>
Queue<T>& Queue<T>::operator=(Queue&& source)
{
    std::swap(head, source.head);
    std::swap(tail, source.tail);
    std::swap(sz, source.sz);
    return *this;
}

template <class T>
T& Queue<T>::front()
{
    if (!head) throw std::out_of_range("front(): queue is empty");
    return head -> val;
}

template <class T>
const T& Queue<T>::front() const
{
    if (!head) throw std::out_of_range("front(): queue is empty");
    return head -> val;
}

template <class T>
T& Queue<T>::back()
{
    if (!head) throw std::out_of_range("back(): queue is empty");
    return tail -> val;
}

template <class T>
const T& Queue<T>::back() const
{
    if (!head) throw std::out_of_range("back(): queue is empty");
    return tail -> val;
}

template <class T>
void Queue<T>::enqueue(const T& val)
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
        // Push the node at back
        newnode -> prev = tail;
        tail -> next = newnode;
        tail = newnode;
    }
    ++sz;
}

template <class T>
void Queue<T>::enqueue(T&& val)
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
        // Push the node at back
        newnode -> prev = tail;
        tail -> next = newnode;
        tail = newnode;
    }
    ++sz;
}

template <class T>
void Queue<T>::dequeue()
{
    if (!head) throw std::out_of_range("pop(): can not pop from empty queue");
    Node* temp = head;
    if (head == tail)
    {
        // There is only one node in the list
        head = nullptr;
        tail = nullptr;
        delete temp;
    }
    else
    {
        head = head -> next;
        head -> prev = nullptr;
        delete temp;
    }
    --sz;
}

template <class T>
void Queue<T>::clear()
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
void Queue<T>::print() const
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