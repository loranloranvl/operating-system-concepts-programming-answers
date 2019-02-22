#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "collatz.c"

int main(int argc, char **argv) {
    int n = atoi(argv[1]);
    const char *SHM_NAME = "collatz_share";
    const int SHM_SIZE = 4096;

    pid_t pid = fork();
    if (pid == 0) {
        // child
        char* sequence = collatz_sequence(n);
        int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SHM_SIZE);
        void *ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        sprintf(ptr, "%s", sequence);
        free(sequence);
    }
    else if (pid > 0) {
        // parent
        int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SHM_SIZE);
        void *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        wait(NULL);
        printf("%s",(char *)ptr);
        shm_unlink(SHM_NAME);
    }

    return 0;
}

