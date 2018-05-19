#include <pthread.h>
#include <semaphore.h>

sem_t SCR1, SCR2;
pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;

