#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct Nums {
    int* arr;
    int length;
} Nums;

#define WORKER_NUM 3

double avg = 0;
double min = INFINITY;
double max = -1 * INFINITY;

void* get_avg(void* param);
void* get_min(void* param);
void* get_max(void* param);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "not enough arguments\n");
        return 1;
    }
    Nums series = {.length = argc - 1};
    series.arr = malloc(sizeof(int) * series.length);
    for (int i = 1; i < argc; ++i) {
        int num = atoi(argv[i]);
        series.arr[i - 1] = num;
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    void* (*workers[WORKER_NUM])(void*) = {
        get_avg,
        get_max,
        get_min
    };
    pthread_t threads[WORKER_NUM];

    for (int i = 0; i < WORKER_NUM; ++i) {
        pthread_t tid;
        pthread_create(&tid, &attr, workers[i], &series);
        threads[i] = tid;
    }

    for (int i = 0; i < WORKER_NUM; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("average: %f\n", avg);
    printf("maximum: %f\n", max);
    printf("mininum: %f\n", min);

    return 0;
}

void* get_avg(void* param) {
    Nums* pseries = param;
    for (int i = 0; i < pseries->length; ++i) {
        avg += 1.0 * pseries->arr[i] / pseries->length;
    }
}

void* get_min(void* param) {
    Nums* pseries = param;
    for (int i = 0; i < pseries->length; ++i) {
        if (pseries->arr[i] < min) {
            min = pseries->arr[i];
        }
    }
}

void* get_max(void* param) {
    Nums* pseries = param;
    for (int i = 0; i < pseries->length; ++i) {
        if (pseries->arr[i] > max) {
            max = pseries->arr[i];
        }
    }
}