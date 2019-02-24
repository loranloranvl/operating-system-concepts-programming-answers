#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HISTORY_SIZE 10

typedef struct Cmd {
    int argc;
    char** args;
} Cmd;

typedef struct History {
    Cmd cmds[HISTORY_SIZE];
    int indexes[HISTORY_SIZE];
    int most_recent;
} History;

// read a line from terminal and split it with ' '
int read_line(char** args);

void inspect_history(History* h, char* index);
void execute(int argc, char** args, History* h);
void show_cmd(Cmd* cmd);
void show_history(History* h);

int main() {
    History herstory = {.most_recent = -1};
    bool should_run = true;
    printf("osh> ");
    while (should_run) {
        // char* args[80];
        char** args = malloc(sizeof(char*) * 80);
        int argc = read_line(args);

        if (argc >= 1 && strcmp(args[0], "quit") == 0) {
            return 0;
        }

        if (argc >= 1 && args[0][0] == '!') {
            inspect_history(&herstory, args[0]+1);
            continue;
        }

        if (argc >= 1 && strcmp(args[0], "history") == 0) {
            show_history(&herstory);
            continue;
        }

        execute(argc, args, &herstory);
    }
}

void execute(int argc, char** args, History* h) {
    int next = (h->most_recent + 1) % HISTORY_SIZE;
    // most recent index
    int mri = 0;
    if (h->most_recent != -1) {
        mri = h->indexes[h->most_recent];
    }
    Cmd next_cmd = {argc, args};
    if (mri > 10) {
        free(h->cmds[next].args);
    }
    h->cmds[next] = next_cmd;
    h->indexes[next] = mri + 1;
    h->most_recent = next;

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

void inspect_history(History* h, char* index) {
    if (h->most_recent == -1) {
        printf("No commands in history\nosh> ");
        return;
    }

    Cmd* target;
    if (strcmp(index, "!") == 0) {
        // most recent cmd
        target = &(h->cmds[h->most_recent]);
    } else {
        int target_index = atoi(index);
        int i;
        for (i = 0; i < HISTORY_SIZE; ++i) {
            if (h->indexes[i] == target_index) {
                target = &(h->cmds[i]);
                break;
            }
        }
        if (i == HISTORY_SIZE) {
            printf("No commands in history\nosh> ");
            return;
        }
    }

    printf("osh> ");
    show_cmd(target);
    execute(target->argc, target->args, h);
}

void show_cmd(Cmd* cmd) {
    for (int i = 0; i < cmd->argc; ++i) {
        printf("%s ", cmd->args[i]);
    }
    printf("\n");
}

void show_history(History* h) {
    if (h->most_recent == -1) {
        printf("No commands in history\nosh> ");
        return;
    }

    int end_point = 0;
    int mri = h->indexes[h->most_recent];
    if (mri > 10) {
        end_point = (h->most_recent + 1) % HISTORY_SIZE;
    }
    for (int i = h->most_recent; i != end_point; i = (i - 1 + HISTORY_SIZE) % HISTORY_SIZE) {
        printf("%d ", h->indexes[i]);
        show_cmd(&(h->cmds[i]));
    }
    printf("%d ", h->indexes[end_point]);
    show_cmd(&(h->cmds[end_point]));
    printf("osh> ");
}
