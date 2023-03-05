#ifndef SRC_SORTED_MATRIX_H
#define SRC_SORTED_MATRIX_H

#include <cmath>

using namespace std;

typedef struct Cell {
    long i;
    long j;
    long w;
    long h;
} Cell;

Cell select(double (*matrix)(long, long), int n, int m, long k);

#endif
