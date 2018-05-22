#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <fstream>

using namespace std;

/* Treads */
pthread_t p1;
pthread_t p2;
pthread_t p3;
pthread_t p4;
pthread_t p5;
pthread_t p6;

//unsigned short k = 10000; // time for sleep

ofstream fout("Tread.log");
//Queue *MyQueue = new Queue;


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

/*
Atomic function which include atomic operations and to do operations with variables
*/
void atomic_operations(){

	cout <<"Get in to atomic operations" << endl;
	 __sync_fetch_and_add (&var_int_1, 1);
	 __sync_add_and_fetch (&var_int_2, 1);

	__sync_fetch_and_or(&var_unsg_1, 2);
	__sync_fetch_and_and(&var_unsg_2, 3);

	__sync_xor_and_fetch (&var_long_1, 4);
	__sync_nand_and_fetch (&var_long_2, 5);

	__sync_bool_compare_and_swap(&var_long_uns_1, 2, 3);
	__sync_val_compare_and_swap(&var_long_uns_2, 2, 3);
}

struct t_elem {
	/* Посилання на наступний елемент черги */
	struct t_elem* next;
	/* інформаційні поля елементу черги */
	int number;
};

	/* Черга елементів */
struct t_elem* beg_q=NULL;
struct t_elem* end_q=NULL;

void add_elem()
{
/* Створення нового елементу черги */
        struct t_elem*  p = (struct t_elem*)malloc(sizeof(struct t_elem));
	p->next = NULL;

	if (beg_q == NULL) {
	    p->number = 0;
	    beg_q = p;
	}
	else {
	    p->number = end_q->number+1;
	    end_q->next = p;
	}
	end_q = p;
}

void* get_elem()
{
	void* p=NULL;

/* Перевіряємо чи є елементи в черзі */
                if (beg_q == NULL)
                        p = NULL;
                else {
/* Отримуємо з голови черги її поточний елемент черги для подальшої обробки */
                        p = beg_q;
/* Видаляємо цей елемент із черги */
                        beg_q = beg_q->next;
                }
	return p;
}

/* Функція is_full повертає 1, якщо черга є повною */
int is_full()
{
	return curr_queue_length >= max_queue_length;
}

/* Функція is_empty повертає 1, якщо черга є пустою */
bool is_empty()
{
	return curr_queue_length <= 0;
}

int number_of_full = 0;
bool cond = true;

void *tread1 (void *unused){
	int value;

	int _count = 0;
	while (cond)
	{
		pthread_mutex_lock(&MCR1);
		fout << "P1 mutex lock\n";

			sem_getvalue(&SCR21, &value);
			if (value == 0) sem_post(&SCR21);
			sem_getvalue(&SCR21, &value);
			cout << "P1 The value of the semaphor is " << value << "\n";

			if (is_empty())	{
				   pthread_cond_wait (&Sig1, &MCR1);
				   printf ("P1 Waiting for Sig1 and MCR1\n");
			}

				curr_queue_length--;
				fout << "Consumer thread 1: Pop element = " << beg_q->number <<  " ; current queue length = " << curr_queue_length <<" length\n";
				get_elem();

					if (is_empty()) _count++;
						if (_count >= 2){
							fout << "P1 CR1 Empty two times!\n";
							cond = false;
						}

				fout << "P1 mutex unlock\n";
		pthread_mutex_unlock (&MCR1);
		pthread_cond_signal(&Sig2);
	}
		printf ("P1 goes die\n");
		fout << ("Consumer thread_1  stopped and cancel other !!!! \n");

	    return NULL;
}

void *tread2 (void *unused){
		//int value;
		while (cond)
			{
				pthread_mutex_lock(&MCR1);
				pthread_cond_wait (&Sig21, &MCR1);
					fout << "P2 mutex lock\n";

							if (is_full())
							  {
							  printf ("P2 Waiting for Sig2 and MCR1\n");
							  pthread_cond_wait (&Sig2, &MCR1);
							  }

									add_elem();
									curr_queue_length++;
									fout <<"Producer thread 2: Push element = " << end_q->number << " Created; current queue length = " << curr_queue_length << endl;

									if (is_full())   number_of_full++;
									if (number_of_full >= 2){
										fout << "P2 CR1 full two times!" << endl;
										cond = false;
									}

									fout << "P2 use CR2\n" << var_int_1
												<< " " << var_int_2
												<< " " << var_unsg_1
												<< " " << var_unsg_2
												<< " " << var_long_1
												<< " " << var_long_2
												<< " " << var_long_uns_1
												<< " " << var_long_uns_2 << "\n";
									fout << "P2 modef CR2\n";
									atomic_operations();

					fout << "P4 mutex unlock\n";
					pthread_mutex_unlock (&MCR1);
					pthread_cond_signal(&Sig1);
					pthread_cond_signal(&Sig22);
			}
			printf ("P2 goes die\n");
			fout << ("Producer thread_4  stopped and cancel other !!!! \n");

	return NULL;
}

void *tread3 (void *unused){
	while (1)
		{
			pthread_mutex_lock(&MCR1);
			fout << "P3 mutex lock\n";

			fout << "P3 modef CR2\n";
			atomic_operations();

			pthread_cond_signal(&Sig21);

			fout << "P3 modef CR2\n";
			atomic_operations();

			fout << "P3 mutex unlock\n";
			pthread_mutex_unlock(&MCR1);
		}
	return NULL;
}

void *tread4 (void *unused){
	int value;
	while (cond)
		{
			sem_getvalue(&SCR21, &value);
			cout << "P4 The value of the semaphor is " << value << endl;
			if (value == 1 ){
				pthread_mutex_lock(&MCR1);
				fout << "P4 mutex lock\n";

						if (is_full())
						  {
						  printf ("P4 Waiting for Sig2 and MCR1\n");
						  pthread_cond_wait (&Sig2, &MCR1);
						  }

								add_elem();
								curr_queue_length++;
								fout <<"Producer thread 4: Push element = " << end_q->number << " Created; current queue length = " << curr_queue_length << endl;

								if (is_full())   number_of_full++;
								if (number_of_full >= 2){
									fout << "P4 CR1 full two times!" << endl;
									cond = false;
								}

				fout << "P4 mutex unlock\n";
				pthread_mutex_unlock (&MCR1);
				pthread_cond_signal(&Sig1);
			}
		}
		printf ("P4 goes die\n");
		fout << ("Producer thread_4  stopped and cancel other !!!! \n");

		return NULL;
}

void *tread5 (void *unused){
int value;
int _count = 0;
	while (cond)
		{
			pthread_mutex_lock(&MCR1);
			pthread_cond_wait (&Sig21, &MCR1);
			fout << "P5 mutex lock\n";

				sem_getvalue(&SCR21, &value);
				if (value == 1){
					if (sem_wait(&SCR21)) { printf("sem_trywait (tread5): failed: %s\n", strerror(errno)); }
				}
				sem_getvalue(&SCR21, &value);
				cout << "P5 The value of the semaphor is " << value << endl;

				if (is_empty()) {
					pthread_cond_wait (&Sig1, &MCR1);
					fout << "P5 Waiting for Sig1 and MCR1";
				}

			    	curr_queue_length--;
					fout << "Consumer thread 5: Pop element = " << beg_q->number <<  " ; current queue length = " << curr_queue_length <<" length\n";
			    	get_elem();

					if (is_empty())  _count++;
					if (_count >= 2){
						fout << "P5 Empty two times!" << endl;
						cond = false;
					}

					fout << "P5 use CR2\n" << var_int_1
									<< " " << var_int_2
									<< " " << var_unsg_1
									<< " " << var_unsg_2
									<< " " << var_long_1
								    << " " << var_long_2
									<< " " << var_long_uns_1
									<< " " << var_long_uns_2 << "\n";
									fout << "P5 modef CR2\n";
						atomic_operations();

			fout << "P5 mutex unlock\n";
			pthread_mutex_unlock (&MCR1);
			pthread_cond_signal(&Sig2);
		}

		printf ("P5 goes die\n");
		fout << ("Consumer thread_5  stopped and cancel other !!!! \n");
		return NULL;
}

void *tread6 (void *unused){
	while (1)
		{
		pthread_mutex_lock(&MCR1);
			pthread_cond_wait (&Sig21, &MCR1);
		fout << "P6 mutex lock\n";
					fout << "P6 modef CR2 after Sig21\n";
					atomic_operations();
			pthread_cond_wait (&Sig22, &MCR1);
					fout << "P6 modef CR2 after Sig22\n";
					atomic_operations();
		fout << "P6 mutex unlock\n";
		pthread_mutex_unlock (&MCR1);
								//pthread_cond_signal(&Sig2);
		}
	return NULL;
}

int main()
{
	if (sem_init(&SCR21,0,0) == -1) { printf("sem_init: failed: %s\n", strerror(errno)); }

	fout << "Program begin\n";
   for(int i=0; i<2; i++) {
	   add_elem();
      fout << "Added first element =" << i << endl;
      curr_queue_length++;
	}

   pthread_create (&p1, NULL, &tread1, NULL);
   pthread_create (&p2, NULL, &tread2, NULL);
   pthread_create (&p3, NULL, &tread3, NULL);
   pthread_create (&p4, NULL, &tread4, NULL);
   pthread_create (&p5, NULL, &tread5, NULL);
   pthread_create (&p6, NULL, &tread5, NULL);

   pthread_join (p4, NULL);

   pthread_cancel(p5);
   pthread_cancel(p1);
   pthread_cancel(p2);
   pthread_cancel(p3);
   pthread_cancel(p6);

	fout << "\nEnd program!" << endl;

	fout.close();
	return 0;
}
