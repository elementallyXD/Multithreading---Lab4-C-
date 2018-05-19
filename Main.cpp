#include "Tread.h"

using namespace std;

sem_t SCR1, SCR2;
pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	int info;
	//pthread_t P1,P2,P3,P4,P5,P6;
	sem_init(&SCR1,0,5);
	sem_init(&SCR2,0,0);
	ofstream fout("Tread.log");
	Queue *MyQueue = new Queue;
	CreateQueue(MyQueue);
	fout << "Created Queue" << endl;

	info = 442;
	Push(info, MyQueue, fout);
	Pop(MyQueue, fout);

	ClearQueue(MyQueue);
	delete MyQueue->Head;
	delete MyQueue;
	fout << "Queue Deleted" << endl;

	fout.close();
	return 0;
}
