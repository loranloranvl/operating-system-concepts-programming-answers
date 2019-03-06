#include <stdlib.h>
#include <stdio.h>
#include "manager.h"

int allocate_map(PidMap* pmap, int lower_bound,
        int upper_bound) {
    pmap->length = upper_bound - lower_bound + 1;
    pmap->availabilities = malloc(sizeof(bool) * pmap->length);

    if (pmap->availabilities == NULL) {
        fprintf(stderr, "pid map memory allocation failed\n");
        return -1;
    }
    for (int i = 0; i < pmap->length; ++i) {
        pmap->availabilities[i] = 1;
    }

    pmap->lower_bound = lower_bound;
    pmap->upper_bound = upper_bound;

    return 0;
}

int allocate_pid(PidMap* pmap) {
    for (int i = 0; i < pmap->length; ++i) {
        if (pmap->availabilities[i]) {
            pmap->availabilities[i] = 0;
            return pmap->lower_bound + i;
        }
    }

    fprintf(stderr, "no pid available");
    return -1;
}

void release_pid(PidMap* pmap, int pid) {
    pmap->availabilities[pid - pmap->lower_bound] = 1;
}