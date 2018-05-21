#include <iostream>
#include <fstream>

struct Queue
{
    int x;
    Queue *Next, *Head, *Tail;
};

void CreateQueue(Queue *MyQueue);
int Push(int x,Queue *&MyQueue);
int  Pop(Queue *MyQueue);
void ClearQueue(Queue *MyQueue);
void Print(Queue *MyQueue, std::ofstream &f);
int Action(int Pop);
bool EmptyQueue(Queue *MyQueue);
