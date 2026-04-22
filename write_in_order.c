#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define THREADS 2 
sem_t sem1, sem2, sem3;

void* first(void *arg)
{   
    sem_wait(&sem1);
    printf("First!\n");
    sem_post(&sem2);
    return NULL;
}

void* second(void *arg)
{
    sem_wait(&sem2);
    printf("Second!\n");
    sem_post(&sem3);
    return NULL;
}

void* third(void *arg)
{
    sem_wait(&sem3);
    printf("Third!\n");
    sem_post(&sem1);
    return NULL;
}

int main()
{
    void* (*funcs[])(void*) = { first, second, third };
    if (sem_init(&sem1, 0, 1) == -1) printf("Error in sem_init.\n");  // One place
    if (sem_init(&sem2, 0, 0) == -1) printf("Error in sem_init.\n");
    if (sem_init(&sem3, 0, 0) == -1) printf("Error in sem_init.\n");
    pthread_t threads[THREADS];
    int i,j;

    for ( i = j = 0; i < THREADS && j < 3; i++, j++)
    {
        pthread_create(&threads[i], NULL, funcs[j], NULL);
        if (j == 2) j = -1;
    }

    for ( i = j = 0; i < THREADS && j < 3; i++, j++)
    {
        pthread_join(threads[i], NULL);
        if (j == 2) j = -1;
    }

    sem_destroy(&sem1);
    sem_destroy(&sem2);

}
