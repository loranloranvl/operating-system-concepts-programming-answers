#include <string.h>
#include <stdlib.h> 

char * collatz_sequence(int n) {
    const int SEQUENCE_SIZE = 4095;
    // char sequence[SEQUENCE_SIZE];
    char * sequence = malloc(SEQUENCE_SIZE * sizeof(char));
    sprintf(sequence, "%d ", n);
    while (n > 1 && n < SEQUENCE_SIZE) {
        if (n % 2) {
            n = 3 * n + 1;
        } else {
            n = n / 2;
        }
        sprintf(sequence + strlen(sequence), "%d ", n);
    }
    sprintf(sequence + strlen(sequence), "\n");
    return sequence;
}