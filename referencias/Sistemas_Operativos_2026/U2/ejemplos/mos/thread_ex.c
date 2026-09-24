/*
 *  Modern Operating System 5th ed
 *  
 *   Fig 2-14
 *
 * compilar con gcc -Wall -pthread -o thread_ex thread_ex.c 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 10

void *print_hello_world(void *tid)
{
        /* This function prints the thread’s identifier and then exits. */
        printf("Hello World. Greetings from thread %ld\n", (long) tid);
        pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
        /* The main program creates 10 threads and then exits. */
        pthread_t threads[NUMBER_OF_THREADS];
        int status;
        long i;
        for(i=1; i <= NUMBER_OF_THREADS; i++) {
                printf("Main here. Creating thread %ld\n", i);
                status = pthread_create(&threads[i], NULL, print_hello_world, (void *)i);
                if (status != 0) {
                    printf("Oops. pthread create returned error code %d\n", status);
                    exit(-1);
                }
    }
    exit(0);
}
