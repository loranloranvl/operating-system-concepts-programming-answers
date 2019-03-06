#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define THREAD_NUM 4

typedef struct Shared {
    int sup;
    bool* prime_bitmap;
} Shared;

typedef struct Param {
    Shared* psrd;
    int thread_index;
} Param;

void* is_prime(void* param);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "not enough arguments\n");
        return 1;
    }

    int sup = atoi(argv[1]);

    Shared srd = {
        .sup = sup,
        .prime_bitmap = malloc(sizeof(bool) * sup)
    };

    memset(srd.prime_bitmap, 1, sizeof(bool) * sup);

    pthread_t threads[THREAD_NUM];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i < THREAD_NUM; ++i) {
        Param* pprm = malloc(sizeof(Param));
        pprm->psrd = &srd;
        pprm->thread_index = i;
        pthread_create(threads + i, &attr, is_prime, pprm);
    }

    for (int i = 0; i < THREAD_NUM; ++i)
        pthread_join(threads[i], NULL);

    for (int i = 2; i < sup; ++i) {
        if (srd.prime_bitmap[i]) printf("%d ", i);
    }

    printf("\n");
}

void* is_prime(void* param) {
    Param* pprm = param;
    Shared* psrd = pprm->psrd;
    int ti = pprm->thread_index;

    for (int num = ti; num < psrd->sup; ++num) {
        for (int divisor = 2; divisor <= sqrt(num); ++divisor) {
            if (num % divisor == 0) {
                psrd->prime_bitmap[num] = 0;
            }
        }
    }
}