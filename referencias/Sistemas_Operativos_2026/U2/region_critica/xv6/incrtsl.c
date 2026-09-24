/*
 *   Esta version usa instrucciones atómicas de assembler.
 *
 */


#include "kernel/types.h"
#include "user/user.h"
#include "kernel/riscv.h"
#include "kernel/memlayout.h"

#define K 100 * 1000 * 1000

struct shared_var_t {
    volatile uint64 x;
    int lock;
};

int
tsl(volatile int *lock)
{
    int old;

    asm volatile(
        "amoswap.w.aq %0, %2, (%1)"
        : "=r"(old)
        : "r"(lock), "r"(1)
        : "memory");

    return old;
}


void
lock_acquire(volatile int *lock)
{
    while (tsl(lock) != 0)
        ;
}

void
lock_release(volatile int *lock)
{
    asm volatile(
        "amoswap.w.rl x0, x0, (%0)"
        :
        : "r"(lock)
        : "memory");
}




int main() {
    int pid;
    struct shared_var_t  *const shared_var_ptr = (struct shared_var_t *) (SHARED_PAGE);
    shared_var_ptr->x=0;
    shared_var_ptr->lock=0;

    pid = fork();
    if (pid < 0) {
        fprintf(2,"fork failed");
        exit(2);
    }


    if (pid == 0) {
        // --- CHILD PROCESS ---

        for (uint64 i = 0; i < K; ++i)
        {
            lock_acquire(&shared_var_ptr->lock);
            shared_var_ptr->x++;
            lock_release(&shared_var_ptr->lock);
        }

        exit(0);

    } else {
        // --- PARENT PROCESS ---
        uint64 start, end;

        // Start tracking time right before the loop
        start = gettime(1); // Get starting time in nanoseconds        

        for (uint64 i = 0; i < K; ++i)
        {
            lock_acquire(&shared_var_ptr->lock);
            shared_var_ptr->x++;
            lock_release(&shared_var_ptr->lock);
        }

        // Stop tracking time right after the loop finishes
        end = gettime(1); // Get ending time in nanoseconds

        // Wait for child to clean up cleanly
        wait(0); 

        // Calculate total elapsed time in miliseconds
        uint64 elapsed_time = (end - start) / 1000000;
    
        printf("x=%ld\n",shared_var_ptr->x);    
        printf("Completed in %ld miliseconds.\n", elapsed_time);
    }
    exit(0);
}

