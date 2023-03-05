#ifndef SRC_SQUARE_H
#define SRC_SQUARE_H

using namespace std;

typedef struct Point {
    double *p;
    int dim;
    int color;
    int id;
    Point *left;
    Point *right;
} Point;

typedef struct {
    Point corner;
    double d;   //side
} Square;

typedef struct {
    Point p;
    Point q;
} Pair;

Square smallest_square(Pair *pairs, int n);

#endif
