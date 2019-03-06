#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 2

static long num_steps = 100000;
double step;

int main() {
    int nthreads;
    double pi, sum[NUM_THREADS];
    step = 1.0 / (double) num_steps;

    // for (i = 0; i < num_steps; i++) {
    //     x = (i + 0.5) * step;
    //     sum = sum + 4.0 / (1.0 + x * x);
    // }
    
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        double x;
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;
        for (int i = id; i < num_steps; i += nthrds) {
            x = (i + 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }

    for (int i = 0; i < nthreads; ++i) {
        pi += sum[i] * step;
    }

    printf("pi = %lf\n", pi);
}