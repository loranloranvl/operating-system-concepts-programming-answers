#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "quicksort.h"

typedef struct {
    int* arr;
    int length;
} ArrWithLength;

int raw[1024];
int sorted[1024];

void* sorter(void* param);
void* merger(void* param);

int main() {
    FILE* pfile = fopen("input.txt", "r");
    int length = 0;
    for (int i = 0; fscanf(pfile, "%d", raw + i) != EOF; ++i)
        length++;

    // thread params for the sorters
    pthread_t sorters[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // thread params for the merger
    pthread_t merger_t;
    ArrWithLength* pawls[3];
    for (int i = 0; i < 3; ++i)
        pawls[i] = malloc(sizeof(ArrWithLength));
    pawls[2]->arr = sorted;
    pawls[2]->length = 0;

    for (int i = 0; i < 2; ++i) {
        ArrWithLength* pawl = malloc(sizeof(ArrWithLength));
        pawl->arr = i ? raw+length/2 : raw;
        pawl->length = i ? length-length/2 : length/2;
        memcpy(pawls[i], pawl, sizeof(ArrWithLength));
        pthread_create(sorters + i, &attr, sorter, pawl);
    }

    for (int i = 0; i < 2; ++i)
        pthread_join(sorters[i], NULL);

    pthread_create(&merger_t, &attr, merger, pawls);
    pthread_join(merger_t, NULL);

    for (int i = 0; i < pawls[2]->length; ++i)
        printf("%d ", pawls[2]->arr[i]);

    printf("\n");
}

void* sorter(void* param) {
    ArrWithLength* pawl = param;
    int* arr = pawl->arr;
    int length = pawl->length;

    quick_sort(arr, length);
}

void* merger(void* param) {
    ArrWithLength** pawls = param;
    int* arr0 = pawls[0]->arr;
    int* arr1 = pawls[1]->arr;
    int* arr_dst = pawls[2]->arr;
    int length0 = pawls[0]->length;
    int length1 = pawls[1]->length;

    int i = 0;
    int j = 0;
    int k = pawls[2]->length;

    while (i < length0 && j < length1) {
        if (arr0[i] < arr1[j]) {
            arr_dst[k] = arr0[i];
            i++;
        } else {
            arr_dst[k] = arr1[j];
            j++;
        }
        k++;
    }

    while (i < length0) {
        arr_dst[k] = arr0[i];
        i++;
        k++;
    }

    while (j < length1) {
        arr_dst[k] = arr1[j];
        j++;
        k++;
    }

    pawls[2]->length = k;
}