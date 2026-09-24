/*
 * compilar con:
 *    gcc -Wall -O0 -o incralt incralt.c
 *     se cuelga si se usa optimización. 
 */
#include <pthread.h>
#include <stdio.h>
#define K 100 * 1000 * 1000

volatile int x=0;
volatile int turn=0;

void* threadfunc0(void* arg) {
    (void) arg;
    for (int i = 0; i < K; ++i)
    {
        while(turn != 0);
        x++;
        turn=1;
    }
    return 0;
}

void* threadfunc1(void* arg) {
    (void) arg;
    for (int i = 0; i < K; ++i)
    {
        while( turn != 1 );
        x++;
        turn=0;
    }
    return 0;
}

int main() {
    pthread_t th[2];

    pthread_create(&th[0], NULL, threadfunc0, NULL);
    pthread_create(&th[1], NULL, threadfunc1, NULL);
    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);
    printf("%u\n", x);
}


