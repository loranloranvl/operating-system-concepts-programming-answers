#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* estimate_pi(void* param);

double pi = 0;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "not enough arguments\n");
        return 1;
    }
    int points_num = atoi(argv[1]);

    srand(233);

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, estimate_pi, &points_num);

    pthread_join(tid, NULL);

    printf("estimated pi: %f\n", pi);
}

void* estimate_pi(void* param) {
    int points_num = *(int *) param;
    int in_circle_num = 0;
    for (int i = 0; i < points_num; ++i) {
        double x = 2.0 * rand() / RAND_MAX - 1;
        double y = 2.0 * rand() / RAND_MAX - 1;
        if (x*x + y*y <= 1.0) {
            in_circle_num++;
        }
    }
    pi = 4.0 * in_circle_num / points_num;
}