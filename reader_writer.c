/*
 Exercise 3: Readers-Writers Problem
    Implement a solution to the Readers-Writers problem where multiple readers can simultaneously access a shared resource, but writers require exclusive access.
    Use semaphores to manage access.                                          
  so multiple can read, one can write? no write while reading */
/*---------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 6

sem_t mutex, action;
int reading = 0;
int ressource = 0;

void* readfunction(void *arg) // many can read at once its fine, ONLY ONE CAN WRITE ON THE RESSOURCE
{
    sem_wait(&mutex);
    reading++;
    if (reading == 1)    sem_wait(&action);  // there are readers, stop writer from using wait by using it aimlessly
    sem_post(&mutex);

    printf("Reading : %d\n", ressource);    // multiple can read, this is fine, should be available for all threads at once
    sleep(1);

    sem_wait(&mutex);
    reading--;
    if (reading == 0) sem_post(&action);    // when the last reader is out, we can allow writer by giving them the action semaphore use
    sem_post(&mutex);
    
    return NULL;
}

void* writefunction(void *arg)
{
    sem_wait(&action);
    sem_wait(&mutex);
    ressource += 10;
    printf("Writing on ressource...\n");
    sleep(1);
    sem_post(&mutex);
    sem_post(&action);

    return NULL;
}

void clean()
{
    sem_destroy(&action);
    sem_destroy(&mutex);
}

int main()
{
    pthread_t thread[THREADS];
    sem_init(&action, 0, 1);
    sem_init(&mutex, 0, 1);

    for (int i=0; i< THREADS; i++)
    {
        if (i%2 == 0)
        {
            if (pthread_create(&thread[i], NULL, writefunction, NULL) != 0)
            {
                perror("Error terminating thread.\n");
                exit(1);
            }
            
        }
        else 
        {
            if ( pthread_create(&thread[i], NULL, readfunction, NULL) != 0)
            {
                perror("Error in thread creation.\n");
                exit(1);
            }
        }
    }   
    

    for (int i=0; i<THREADS; i++) pthread_join(thread[i], NULL);
    clean();
    printf("Ressource final value from main: %d\n", ressource);
    return 0;
}



