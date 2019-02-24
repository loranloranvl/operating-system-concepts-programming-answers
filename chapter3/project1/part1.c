#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// read a line from terminal and split it with ' '
int read_line(char** args);

int main() {
    bool should_run = true;
    printf("osh> ");
    while (should_run) {
        char* args[80];
        int argc = read_line(args);

        if (argc >= 1 && strcmp(args[0], "quit") == 0) {
            return 0;
        }

        bool run_background = strcmp(args[argc - 1], "&") == 0;
        if (run_background) {
            args[argc - 1] = NULL;
        }

        pid_t pid = fork();
        if (pid == 0) {
            // child
            execvp(args[0], args);
        } else {
            if (run_background) {
                // printf("%d\n", pid);
                printf("osh> ");
            } else {
                waitpid(pid, NULL, 0);
                printf("osh> ");
            }
        }
    }
}

int read_line(char** args) {
    char word_tmp[80];
    char** wordp = args;
    char* charp = word_tmp;
    char last_char = '\n';

    while (1) {
        char c = getc(stdin);
        if (last_char == '\n' && c == '\n') {
            *charp = '\0';
            if (strcmp(word_tmp, "quit") != 0) {
                printf("osh> ");
            }
        }
        bool is_blank = c == ' ' || c == '\n';

        if (last_char == '\n' && is_blank || last_char == ' ' && c == ' ')
            // ignore duplicated spaces and '\n's
            // take special care to '\n' following ' '
            continue;

        if (is_blank && !(last_char == ' ' && c == '\n')) {
            // end current word
            *charp = '\0';

            // copy word_tmp to target
            *wordp = malloc(sizeof(char)*strlen(word_tmp));
            strcpy(*wordp, word_tmp);

            // get ready to receive the next word
            wordp++;
        }

        if (c == ' ') {
            // initialize charp
            charp = word_tmp;
        } else if (c == '\n') {
            // NULL indicates no more words
            *wordp = NULL;
            return wordp - args;
        } else {
            // place other characters in word_tmp sequentially
            *charp = c;
            charp++;
        }
        last_char = c;
    }
}