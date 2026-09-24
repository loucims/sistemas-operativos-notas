/*
 *  esta version usa  GCC atomic built-ins.
 *  
*/

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/riscv.h"
#include "kernel/memlayout.h"

#define K 100 * 1000 * 1000

struct shared_var_t {
    volatile uint64 x;
};


int main() {
    int pid;
    struct shared_var_t  *const shared_var_ptr = (struct shared_var_t *) (SHARED_PAGE);
    shared_var_ptr->x=0;

    pid = fork();
    if (pid < 0) {
        fprintf(2,"fork failed");
        exit(2);
    }


    if (pid == 0) {
        // --- CHILD PROCESS ---

        for (uint64 i = 0; i < K; ++i)
        {
            (void) __sync_fetch_and_add(&shared_var_ptr->x, 1);
        }

        exit(0);

    } else {
        // --- PARENT PROCESS ---
        uint64 start, end;

        // Start tracking time right before the loop
        start = gettime(1); // Get starting time in nanoseconds        

        for (uint64 i = 0; i < K; ++i)
        {
            (void) __sync_fetch_and_add(&shared_var_ptr->x, 1);
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

