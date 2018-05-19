#include <iostream>
#include <fstream>

struct Queue
{
    int x;
    Queue *Next, *Head, *Tail;
};

void CreateQueue(Queue *MyQueue);
void Push(int x,Queue *&MyQueue, std::ofstream &f);
void Pop(Queue *MyQueue, std::ofstream &f);
void ClearQueue(Queue *MyQueue);
void Print(const Queue *MyQueue, std::ofstream &f);
