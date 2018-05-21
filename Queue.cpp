#include "Queue.h"

using namespace std;

void CreateQueue(Queue *MyQueue)
{
    MyQueue->Head = NULL;
    MyQueue->Tail = NULL;
}

bool EmptyQueue(Queue *MyQueue)
{
	return (MyQueue->Head == NULL);
}

int Push(int x,Queue *&MyQueue)
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
    return element->x;
}

int Pop(Queue *MyQueue)
{
	int p;

	if (MyQueue->Head != NULL)
    {
        Queue *temp = MyQueue->Head->Next;
        p = MyQueue->Head->x;
        MyQueue->Head = temp;
       // f << "Geted element from head" << endl;
    }

	return p;
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

void Print(Queue *MyQueue, ofstream &f)
{
   if (EmptyQueue(MyQueue))
	   return;

   const Queue* element = MyQueue-> Head;

   for (; element; element = element->Next)
   {
     //cout << element->x;
      f << element->x;
   }
   // cout << endl;
   f << endl;
}

int Action(int Pop)
{
	//cout << "Does actions for elem! \n";
	return Pop;
}


