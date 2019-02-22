#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
        execl("/bin/ls","ls", "-l",NULL);
        printf("Line J\n");
        return 1;
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        int status = 233;
        wait(&status);
        printf("Child returned %d\n", WEXITSTATUS(status));
        printf("Child Complete\n");
    }
    return 0;
}