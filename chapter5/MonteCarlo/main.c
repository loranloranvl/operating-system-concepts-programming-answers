#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* estimate_pi(void* param);

int in_circle_num = 0;
pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "not enough arguments\n");
        return 1;
    }
    int points_num = atoi(argv[1]);

    srand(233);

    const int thread_num = 10;
    int task_num = points_num / thread_num;
    pthread_t threads[thread_num];
    pthread_attr_t attr;
    pthread_mutex_init(&mutex, NULL);
    pthread_attr_init(&attr);

    for (int i = 0; i < thread_num; ++i) {
        pthread_t tid;
        pthread_create(&tid, &attr, estimate_pi, &task_num);
        threads[i] = tid;
    }

    for (int i = 0; i < thread_num; ++i)
        pthread_join(threads[i], NULL);

    printf("estimated pi: %f\n", 4.0 * in_circle_num / task_num / thread_num);
}

void* estimate_pi(void* param) {
    int task_num = *(int *) param;
    for (int i = 0; i < task_num; ++i) {
        double x = 2.0 * rand() / RAND_MAX - 1;
        double y = 2.0 * rand() / RAND_MAX - 1;
        if (x*x + y*y <= 1.0) {
            pthread_mutex_lock(&mutex);
            in_circle_num++;
            pthread_mutex_unlock(&mutex);
        }
    }
}