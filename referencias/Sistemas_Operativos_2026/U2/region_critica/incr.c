/* ----------------- incr.c ----------------------*
 * compilar con:
 *   gcc -Wall -o incr incr.c 
 *
 */
#include <pthread.h>
#include <stdio.h>
#define K 1000 * 1000 * 1000

volatile unsigned x=0;

void* threadfunc(void* arg) {
    (void) arg;
    for (int j = K-1; j >=0 ; j--)
    {
        x++;
    }
    return(0);
}

int main() {
    pthread_t th[2];

    for (int i = 0; i < 2; i++)
       pthread_create(&th[i], NULL, threadfunc, NULL);
    for (int i = 0; i < 2; i++)
       pthread_join(th[i], NULL);
    printf("%u\n", x);
}

