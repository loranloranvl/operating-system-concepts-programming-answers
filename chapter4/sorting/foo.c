#include <stdio.h>
#include "quicksort.h"

int main() {
    FILE* f = fopen("input.txt", "r");
    while (1) {
        int scanned;
        int flag = fscanf(f, "%d ", &scanned);
        printf("scanned: %d\tflag: %d\n", scanned, flag);
        if (flag == EOF) {
            fclose(f);
            return 0;
        }
    }
}