#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int** grid;
    int** flags;
} GridFlags;

typedef struct {
    GridFlags* pgf;

    // subgrid values:
    //   0 1 2
    //   3 4 5
    //   6 7 8
    int subgrid;
} SubgridGridFlags;

void* row_checker(void* param);
void* col_checker(void* param);
void* subgrid_checker(void* param);

int main() {
    int** grid = malloc(sizeof(int**) * 9);
    for (int i = 0; i < 9; ++i)
        grid[i] = malloc(sizeof(int*) * 9);
    FILE* fp = fopen("grid.txt", "r");
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            fscanf(fp, "%d", grid[i] + j);

    // check_flags[0]: rows
    // check_flags[1]: cols
    // check_flags[2]: subgrids
    int** check_flags = malloc(sizeof(int**) * 3);
    for (int i = 0; i < 3; ++i)
        check_flags[i] = malloc(sizeof(int*) * 9);
    for (int i = 0; i < 3; ++i)
        memset(check_flags[i], 0, 9 * sizeof(int));

    GridFlags gf = {
        .grid = grid,
        .flags = check_flags
    };

    pthread_t threads[11];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(threads, &attr, row_checker, &gf);
    pthread_create(threads + 1, &attr, col_checker, &gf);
    for (int i = 0; i < 9; ++i) {
        SubgridGridFlags* psgf = malloc(sizeof(SubgridGridFlags));
        psgf->pgf = &gf;
        psgf->subgrid = i;
        pthread_create(
            threads + 2 + i, 
            &attr, 
            subgrid_checker, 
            psgf
        );
    }

    for (int i = 0; i < 11; ++i)
        pthread_join(threads[i], NULL);

    bool bit_ands = 1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 9; ++j) {
            bit_ands &= check_flags[i][j];
        }
    }

    printf(bit_ands ? "valid\n" : "invalid\n");
}

void* row_checker(void* param) {
    GridFlags* pgf = param;
    int** grid = pgf->grid;
    int** flags = pgf->flags;

    for (int i = 0; i < 9; ++i) {
        bool exist[9];
        for (int ii = 0; ii < 9; ++ii)
            exist[ii] = 0;
        for (int j = 0; j < 9; ++j)
            exist[grid[i][j] - 1] = 1;
        bool bit_ands = 1;
        for (int ii = 0; ii < 9; ++ii)
            bit_ands &= exist[ii];
        flags[0][i] = bit_ands;
    }
}

void* col_checker(void* param) {
    GridFlags* pgf = param;
    int** grid = pgf->grid;
    int** flags = pgf->flags;

    for (int j = 0; j < 9; ++j) {
        bool exist[9];
        for (int ii = 0; ii < 9; ++ii)
            exist[ii] = 0;
        for (int i = 0; i < 9; ++i)
            exist[grid[i][j] - 1] = 1;
        bool bit_ands = 1;
        for (int ii = 0; ii < 9; ++ii)
            bit_ands &= exist[ii];
        flags[1][j] = bit_ands;
    }
}

void* subgrid_checker(void* param) {
    SubgridGridFlags* psgf = param;
    GridFlags* pgf = psgf->pgf;
    int** grid = pgf->grid;
    int** flags = pgf->flags;
    int subgrid = psgf->subgrid;

    int rb = subgrid / 3 * 3;
    int cb = subgrid % 3 * 3;

    bool exist[9];
    for (int ii = 0; ii < 9; ++ii)
        exist[ii] = 0;
    for (int i = rb; i < rb + 3; ++i)
        for (int j = cb; j < cb + 3; ++j) {
            exist[grid[i][j] - 1] = 1;
        }
    bool bit_ands = 1;
    for (int ii = 0; ii < 9; ++ii)
        bit_ands &= exist[ii];
    for (int ii = 0; ii < 9; ++ii)
        flags[2][ii] = bit_ands;
}