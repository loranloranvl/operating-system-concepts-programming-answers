#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define NUM_THREADS 4

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "not enough arguments\n");
        return 1;
    }
    int points_num = atoi(argv[1]);
    int in_circle_num[NUM_THREADS];

    srand(233);

    int nthreads;

    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;
        in_circle_num[id] = 0;
        for (int i = id; i < points_num; i += nthrds) {
            double x = 2.0 * rand() / RAND_MAX - 1;
            double y = 2.0 * rand() / RAND_MAX - 1;
            if (x*x + y*y <= 1.0) {
                in_circle_num[id]++;
            }
        }
    }

    int in_circle_sum = 0;
    for (int i = 0; i < nthreads; ++i) {
        in_circle_sum += in_circle_num[i];
    }

    double pi = 4.0 * in_circle_sum / points_num;
    printf("estimated pi: %f\n", pi);
}