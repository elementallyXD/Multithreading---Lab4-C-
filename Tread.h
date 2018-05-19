#include <pthread.h>
#include <semaphore.h>

sem_t SCR21, SCR22;
pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;
