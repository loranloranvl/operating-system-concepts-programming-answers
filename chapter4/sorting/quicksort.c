#include "quicksort.h"

void swap(int* p1, int* p2) {
    *p1 = *p1 ^ *p2;
    *p2 = *p1 ^ *p2;
    *p1 = *p2 ^ *p1;
}

void quick_sort(int* arr, int length) {
    if (length <= 1) return;

    int* ppivot = arr; // partition point
    int* pbegin = arr + length - 1; // inclusive
    int step = -1;

    int* pi;
    while (1) {
        // locate pi to the first element < or > pivot
        for (
            pi = pbegin; 
            pi != ppivot && *pi * step <= *ppivot * step; 
            pi += step
        );

        // if no such element
        if (pi == ppivot) break;

        // else
        swap(pi, ppivot);

        // reverse step direction
        step = 0 - step;

        // set pbegin to one element after original pivot position
        pbegin = ppivot + step;

        // update ppivot since it is swapped
        ppivot = pi;
    }

    // recursively sort [0, pi - 1]
    quick_sort(arr, pi - arr);

    // recursively sort [pi + 1, -1]
    quick_sort(pi + 1, length - 1 - (pi - arr));
}