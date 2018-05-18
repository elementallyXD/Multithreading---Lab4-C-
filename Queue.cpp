#include "Queue.h"

using namespace std;

void CreateQueue(Queue *MyQueue)
{
    MyQueue->Head = NULL;
    MyQueue->Tail = NULL;
}

void Push(int x,Queue *&MyQueue)
{

    Queue *element = new Queue;
    element->x = x;
    element->Next = NULL;
    if (MyQueue->Head == NULL)
        MyQueue->Head = MyQueue->Tail = element;
    else
    {
        (MyQueue->Tail)->Next = element;
        MyQueue->Tail = element;
    }
}

void Pop(Queue *MyQueue)
{
    if (MyQueue->Head != NULL)
    {
        Queue *temp = MyQueue->Head->Next;
        MyQueue->Head = temp;
    }
}

void ClearQueue(Queue *MyQueue)
{
    while (MyQueue->Head != NULL)
    {
        Queue *temp = MyQueue->Head->Next;
        delete MyQueue->Head;
        MyQueue->Head = temp;
    }
}

bool EmptyQueue(const Queue *MyQueue)
{
	return (MyQueue->Head == NULL);
}

void Print(const Queue *MyQueue)
{
   if (EmptyQueue(MyQueue))
	   return;

   const Queue* element = MyQueue-> Head;

   for (; element; element = element->Next)
   {
      cout << element->x;
   }
   cout << endl;
}


