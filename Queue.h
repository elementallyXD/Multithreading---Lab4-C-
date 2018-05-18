#include <iostream>

struct Queue
{
    int x;
    Queue *Next, *Head, *Tail;
};


void CreateQueue(Queue *MyQueue);
void Push(int x,Queue *&MyQueue);
void Pop(Queue *MyQueue);
void ClearQueue(Queue *MyQueue);
void Print(const Queue *MyQueue);
