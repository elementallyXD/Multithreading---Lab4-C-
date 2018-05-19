//#include "Tread.h"
#include <pthread.h>
#include <semaphore.h>
#include "Queue.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;

/* Treads */
pthread_t p1;
pthread_t p2;
pthread_t p3;
pthread_t p4;
pthread_t p5;
pthread_t p6;

unsigned short k = 10000; // time for sleep
int _of_buffer = 0;
int number_of_full = 0;
int length_at_start = 10;

ofstream fout("Tread.log");
Queue *MyQueue = new Queue;


/* Дві спільні змінні, що контролюють довжину черги, що використовується в якості буфера */
int max_queue_length = 20; /* максимально допустима довжина черги */
int curr_queue_length =  0; /* поточна довжина черги */

/* Дві сигнальні (умовні) змінні для сигналізування про непорожність та неповноту черги */
pthread_cond_t  	Sig1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t  	Sig2  = PTHREAD_COND_INITIALIZER;

/*Signal veriables from scheme */
pthread_cond_t		Sig21 = PTHREAD_COND_INITIALIZER;
pthread_cond_t		Sig22 = PTHREAD_COND_INITIALIZER;

/*   Semaphores  */
sem_t SCR21;

/* mutex */
pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;

/*
Atomic variables
*/
int var_int_1 = 0;
int var_int_2 = 1;
unsigned var_unsg_1 = 2;
unsigned var_unsg_2 = 3;
long var_long_1 = 4;
long var_long_2 = 5;
long unsigned var_long_uns_1 = 6;
long unsigned var_long_uns_2 = 7;

/*
Atomic function which include atomic operations and to do operations with variables
*/
void atomic_operations(){

	cout <<"Get in to atomic operations" << endl;
	cout <<"atomic_1" << __sync_fetch_and_add (&var_int_1, 1) << endl;
	cout <<"atomic_2" << __sync_add_and_fetch (&var_int_2, 1)<< endl;

	cout <<"atomic_3" << __sync_fetch_and_or(&var_unsg_1, 2)<< endl;
	cout <<"atomic_4" << __sync_fetch_and_and(&var_unsg_2, 3)<< endl;

	cout <<"atomic_5" <<__sync_xor_and_fetch (&var_long_1, 4)<< endl;
	cout <<"atomic_6" << __sync_nand_and_fetch (&var_long_2, 5)<< endl;

	cout <<"atomic_7" <<__sync_bool_compare_and_swap(&var_long_uns_1, 2, 3)<< endl;
	cout <<"atomic_8" << __sync_val_compare_and_swap(&var_long_uns_2, 2, 3)<< endl;
}

/* Функція is_full повертає 1, якщо черга є повною */
int is_full()
{
	return curr_queue_length >= max_queue_length;
}

/* Функція is_empty повертає 1, якщо черга є пустою */
int is_empty()
{
	return curr_queue_length <= 0;
}

void *tread1 (void *unused){
	while (1) {
	        if (EmptyQueue(MyQueue) == true)
	    		break;
	/* Захоплення м'ютекса черги для додавання до неї нового завдання */
		pthread_mutex_lock (&MCR1);

	/* Чекаємо доки поточний розмір черги дозволить додати ще одне завдання */
		if (is_full())
			pthread_cond_wait (&Sig2, &MCR1);


	/* Додаємо новий елемент до черги тільки у випадку,
	 * якщо масимально допустима довжина черги не перевищена */
		Push(length_at_start+1,MyQueue,fout);
		curr_queue_length++;
		printf("Producer thread: element %d CREATED; current queue length = %d;\n",MyQueue->x,curr_queue_length);

        if (is_full()) {
            number_of_full++;
        }
        if(number_of_full >= 3) break;
	/* Звільнення м'ютекса черги */
		pthread_mutex_unlock (&MCR1);

	/* Повідомляємо про те, що в черзі з'явилося нове завдання.
	 * Якщо потоки-споживачі заблоковані в очікуванні цього сигналу,
	 * то один з них буде розблоковано для обробки нового завдання. */
		pthread_cond_broadcast(&Sig1);

	/* Затримка потоку на задану кількість мікросекунд */
	//	usleep(1);
	    }
	    if(number_of_full >= 3){
            printf("Send signal to died\n");
            pthread_mutex_unlock (&MCR1);
            pthread_cond_broadcast (&Sig1);
        }
        _of_buffer = 1;
        printf("Producer thread_1  stopped !!!\n");
       	pthread_join (p2, NULL);
	    pthread_join (p3, NULL);
	    pthread_join (p4, NULL);
	    pthread_join (p5, NULL);
	    pthread_join (p6, NULL);
	   return NULL;
}

void *tread2 (void *unused){
	while (1)
		{

		}
}

void *tread3 (void *unused){
	while (1)
		{

		}
}

void *tread4 (void *unused){
	while (1)
		{

		}
}

void *tread5 (void *unused){
	while (1)
		{

		}
}

void *tread6 (void *unused){
	while (1)
		{

		}
}


int main()
{
	CreateQueue(MyQueue);
	fout << "Created Queue" << endl;
	if (sem_init(&SCR21,0,1) == -1) { printf("sem_init: failed: %s\n", strerror(errno)); }


	int i;
/* Створення початкової черги із заданою кількостю елементів length_at_start
  перед запуском потоків*/
   for(i=0;i<length_at_start;i++) {
      Push(i, MyQueue, fout);
      curr_queue_length++;
	}

	ClearQueue(MyQueue);
	delete MyQueue->Head;
	delete MyQueue;
	fout << "Queue Deleted" << endl;

	fout.close();
	return 0;
}
