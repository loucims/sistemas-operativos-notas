/* ----------------- incr_atomic.c ----------------------*
 * compilar con:
 *   gcc -Wall -o incr_atomic incr_atomic.c 
 *
 * *  ver https://pubs.opengroup.org/onlinepubs/9799919799/functions/atomic_fetch_add.html#
 *  https://learn.microsoft.com/en-us/cpp/standard-library/atomic-functions
 */
#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#define K 1000 * 1000 * 1000

volatile unsigned x=0;

void* threadfunc(void* arg) {
    (void) arg;
    for (int j = K-1 ; j >=0 ; j--)
    {
        //x++;  es remplazado por 
        //__atomic_fetch_add(&x, 1, __ATOMIC_SEQ_CST);
        atomic_fetch_add(&x,1);
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

