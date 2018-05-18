#include <iostream>
#include "Queue.h"

using namespace std;

int main()
{
	int info;

	Queue *MyQueue = new Queue;
	CreateQueue(MyQueue);

	cout << "Inpute the number: ";
	cin >> info;

	Push(info, MyQueue);
	cout << "Queue :\n";
	Print (MyQueue);
	Pop(MyQueue);
	Print (MyQueue);

	ClearQueue(MyQueue);
	delete MyQueue->Head;
	delete MyQueue;

	return 0;
}
