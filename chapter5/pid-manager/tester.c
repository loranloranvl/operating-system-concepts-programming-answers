#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "manager.h"

#define THREADS_NUM 40
pthread_mutex_t mutex;

void* pid_consumer(void* pmap);

int main() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t threads[THREADS_NUM];
    pthread_mutex_init(&mutex, NULL);


    srand(233);

    PidMap map;
    if(allocate_map(&map, 100, 110) == -1) {
        return 1;
    }

    for (int i = 0; i < THREADS_NUM; ++i) {
        pthread_t tid;
        pthread_create(&tid, &attr, pid_consumer, &map);
        threads[i] = tid;
    }

    for (int i = 0; i < THREADS_NUM; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void* pid_consumer(void* pmap) {
    int life_cycle = 1 + rand() % 2;
    for (int i = 0; i < life_cycle; ++i) {
        sleep(rand() % 5);

        pthread_mutex_lock(&mutex);
        int pid = allocate_pid(pmap);
        pthread_mutex_unlock(&mutex);

        pthread_t tid = pthread_self();
        if (pid == -1) {
            printf("thread %lu failed to get a pid, try later\n", tid);
            --i;
        } else {
            printf("thread %lu got pid %d\n", tid, pid);
            sleep(rand() % 5);

            pthread_mutex_lock(&mutex);
            release_pid(pmap, pid);
            pthread_mutex_unlock(&mutex);

            printf("thread %lu released pid %d\n", tid, pid);
        }
    }
}