#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 5
int nums[SIZE] = {0,1,2,3,4};

int main()
{
    int i;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        for (;;);
    }
    else if (pid > 0) {
        printf("Parent here.\n");
    }
    return 0;
}