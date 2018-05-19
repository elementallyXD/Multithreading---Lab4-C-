//#include <iostream>
#include "Queue.h"
#include "Tread.h"

using namespace std;

int main()
{
	int info;
	ofstream fout("Tread.log");

	Queue *MyQueue = new Queue;
	CreateQueue(MyQueue);
	fout << "Created Queue" << endl;

	cout << "Inpute the number: ";
	cin >> info;

	Push(info, MyQueue, fout);
	Pop(MyQueue, fout);

	ClearQueue(MyQueue);
	delete MyQueue->Head;
	delete MyQueue;
	fout << "Queue Deleted" << endl;

	fout.close();

	return 0;
}
