#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 255
#define READ_END 0
#define WRITE_END 1

void case_reverse(char* s);

int main() {
    int req_fd[2];
    int res_fd[2];
    pid_t pid;

    if (pipe(req_fd) == -1 || pipe(res_fd) == -1) {
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid > 0) {
        // parent | client
        char req_msg[BUFFER_SIZE] = "Hi There";
        char res_msg[BUFFER_SIZE];
        
        close(req_fd[READ_END]);
        close(res_fd[WRITE_END]);

        write(req_fd[WRITE_END], req_msg, strlen(req_msg)+1);
        read(res_fd[READ_END], res_msg, BUFFER_SIZE);
        printf("%s\n", res_msg);

        close(req_fd[WRITE_END]);
        close(res_fd[READ_END]);
    } else {
        // child | server
        char msg[BUFFER_SIZE];

        close(req_fd[WRITE_END]);
        close(res_fd[READ_END]);

        read(req_fd[READ_END], msg, BUFFER_SIZE);
        case_reverse(msg);
        write(res_fd[WRITE_END], msg, strlen(msg)+1);

        close(req_fd[READ_END]);
        close(res_fd[WRITE_END]);
    }

    return 0;
}

void case_reverse(char* s) {
    int len = strlen(s);
    for (int i = 0; i < len; ++i) {
        if ('a' <= s[i] && s[i] <= 'z') {
            s[i] += 'A' - 'a';
        } else if ('A' <= s[i] && s[i] <= 'Z') {
            s[i] += 'a' - 'A';
        }
    }
}