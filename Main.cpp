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
int length_at_start = 2;

ofstream fout("Tread.log");
Queue *MyQueue = new Queue;


/* Дві спільні змінні, що контролюють довжину черги, що використовується в якості буфера */
int max_queue_length = 10; /* максимально допустима довжина черги */
int curr_queue_length =  0; /* поточна довжина черги */

/* Дві сигнальні (умовні) змінні для сигналізування про непорожність та неповноту черги */
pthread_cond_t  	Sig1 = PTHREAD_COND_INITIALIZER;  //not empty (for consumer)
pthread_cond_t  	Sig2  = PTHREAD_COND_INITIALIZER; //not full (for producer)

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

//Sashka
//int max_queue_length = 20; /* максимально допустима довжина черги */
//int curr_queue_length =  0; /* поточна довжина черги */

/* М'ютекс для контролю за чергою */
pthread_mutex_t 	mut_q = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t 	mut_21 = PTHREAD_MUTEX_INITIALIZER;
/* Дві сигнальні (умовні) змінні для сигналізування про непорожність та неповноту черги */
pthread_cond_t  	cond_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t  	cond_not_full  = PTHREAD_COND_INITIALIZER;
/*Signal veriable 21 from scheme 3 */
pthread_cond_t		cond_sig_21 = PTHREAD_COND_INITIALIZER;
/*   Semaphores  */

sem_t scr21;
sem_t scr22;

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
//int flag = -1;
void *tread1 (void *unused){
	while (1) {
		pthread_mutex_lock(&MCR1);
	    if (EmptyQueue(MyQueue) || is_empty())	pthread_cond_wait (&Sig1, &MCR1);
	    pthread_mutex_unlock(&MCR1);

	    	if (sem_post(&SCR21)) { printf("(tread 1)sem_post: failed: %s\n", strerror(errno)); }

	    	int value;
	    	sem_getvalue(&SCR21, &value);
	    	printf ("The value of the semaphor is %d\n", value);

	    	pthread_mutex_lock (&MCR1);

			curr_queue_length--;
			fout << "Consumer thread 1: pop element = " << Action(Pop(MyQueue)) <<  " ; current queue length = " << curr_queue_length << endl;

			pthread_mutex_unlock (&MCR1);
			pthread_cond_broadcast(&Sig2);

			usleep(10000);
			if(_of_buffer){
				 printf("Signal 1 to died\n");
				 break;
			}
	}
		pthread_join (p4, NULL);
		fout << "Producer thread_1  stopped !!!" << endl;
	    return NULL;
}

void *tread2 (void *unused){
	while (1)
			{

			}
	return NULL;
}

void *tread3 (void *unused){
	while (1)
		{

		}
	return NULL;
}

void *tread4 (void *unused){
int count = 0;
	while (1)
		{
					pthread_mutex_lock(&MCR1);
				    if (is_full()) pthread_cond_wait (&Sig2, &MCR1);
				    pthread_mutex_unlock(&MCR1);


						int value;
						sem_getvalue(&SCR21, &value);
						printf ("The value of the semaphor is %d\n", value);

						//if (value >= 1)
				    	if (sem_wait(&SCR21)){printf("sem_wait: failed: %s\n", strerror(errno));}
				    	else
						{
							pthread_mutex_lock (&MCR1);
							fout << "T4 mutex lock\n";
							count++;
							curr_queue_length++;
							fout <<"Producer thread 4:push element = " << Push(curr_queue_length,MyQueue) << " Created; current queue length = " << curr_queue_length << endl;

							if (is_full())   number_of_full++;
							if (number_of_full >= 2) break;

							fout << "T4 mutex unlock\n";
							pthread_mutex_unlock (&MCR1);
							pthread_cond_broadcast(&Sig1);
						}
				  //  if (count == 5) break;
		}
			        _of_buffer = 1;
			       // pthread_cancle(tread5);

				fout << ("Producer thread_4  stopped !!!\n");
				return NULL;
}

void *tread5 (void *unused){
int value;
	while (1)
		{
		pthread_mutex_lock(&MCR1);
		if (EmptyQueue(MyQueue) || is_empty())
			    	pthread_cond_wait (&Sig1, &MCR1);
		pthread_mutex_unlock(&MCR1);

		//else{
			    pthread_mutex_lock (&MCR1);
				curr_queue_length--;
				fout << "Consumer thread 5: pop element = " << Action(Pop(MyQueue)) <<  " ; current queue length = " << curr_queue_length << endl;
				pthread_mutex_unlock (&MCR1);
				pthread_cond_broadcast(&Sig2);
				sem_wait(&SCR21);
				sem_getvalue(&SCR21, &value);
				printf ("The value of the semaphor is %d\n", value);
				if(_of_buffer) break;
				usleep(10000);
			//}
		}
		        fout << "Producer thread_5  stopped !!!" << endl;
			    return NULL;
}

void *tread6 (void *unused){
	while (1)
		{

		}
	return NULL;
}


int main()
{
	CreateQueue(MyQueue);
	fout << "Created Queue" << endl;
	if (sem_init(&SCR21,0,0) == -1) { printf("sem_init: failed: %s\n", strerror(errno)); }

	int i;
/* Створення початкової черги із заданою кількостю елементів length_at_start
  перед запуском потоків*/
   for(i=0;i<length_at_start;i++) {
      Push(i, MyQueue);
      fout << "Added first element =" << i << endl;
      curr_queue_length++;
	}

   pthread_create (&p1, NULL, &tread1, NULL);
   pthread_create (&p4, NULL, &tread4, NULL);
   //pthread_create (&p5, NULL, &tread5, NULL);

   pthread_join (p4, NULL);
   pthread_join (p1, NULL);

  // pthread_join (p5, NULL);

	ClearQueue(MyQueue);
	delete MyQueue->Head;
	delete MyQueue;
	fout << "Queue Deleted" << endl;

	fout.close();
	return 0;
}
