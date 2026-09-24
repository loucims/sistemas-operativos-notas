/*
 *  Basado en OSC 10ed figura 3.8  página 118
 *
 *  modificado para versiones actuales de Linux
 *
 *  compilar con 
 *  $ gcc -Wall -o fig3.8 fig3.8.
 */

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { /* child process */
        execlp("/bin/sleep","ls","30",NULL);
        printf("Terminó");
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
//        wait(NULL);
        printf("Child Complete\n");
    }
    return 0;
}
