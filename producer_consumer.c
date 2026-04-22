#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];

sem_t empty, full, mutex;
int in = 0;
int out = 0;

void* produce(void *arg)
{
    sem_wait(&empty); // 5 can enter -> locked at 5
    sem_wait(&mutex); // but one at time..! -> lock at 1

    int randomized = rand() % 101;
    buffer[in] = randomized;
    printf("Produced %d index %d.\n", buffer[in], in);
    in = (in + 1) % BUFFER_SIZE;

    sem_post(&mutex); // one finshed -> unlock one -> new one can go in
    sem_post(&full); // indicate that there is data entered we have to display

    return NULL;
}

void* consume(void *arg)
{
    sem_wait(&full);
    sem_wait(&mutex);

    printf("Consuming %d index %d.\n", buffer[out], out);
    out = (out + 1) % BUFFER_SIZE;

    sem_post(&mutex);
    sem_post(&empty);  // u can enter new data because we displayed the old one

    return NULL;
}

int main()
{
    srand(time(NULL)); // seed the random number generator
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    
    pthread_t thread1, thread2;
    
    pthread_create(&thread1, NULL, produce, NULL);
    pthread_create(&thread2, NULL, consume, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);
    return 0;
}


