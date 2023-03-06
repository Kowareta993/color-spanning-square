#include "square.h"
#include "SAT2.h"
#include <bits/stdc++.h>
#include "sorted_matrix.h"

struct SAT2 {
    int *a;
    int *b;
    int n;
    int m;
};

Point **sorted_points;

void right(Point *points, int n, int dim, double d) {
    int ptr1 = 0;
    int ptr2 = 0;
    double eps = 0.0001;
    while (ptr2 < n) {
        if (points[ptr2].p[dim] - d - points[ptr1].p[dim] > eps) {
            points[ptr1].right = &points[ptr2];
            ptr1++;
            continue;
        }
        ptr2++;
    }
    while (ptr1 < n) {
        points[ptr1].right = nullptr;
        ptr1++;
    }
}

void left(Point *points, int n, int dim, double d) {
    int ptr1 = n - 1;
    int ptr2 = n - 1;
    double eps = 0.0001;
    while (ptr2 >= 0) {
        if (points[ptr1].p[dim] - d - points[ptr2].p[dim] > eps) {
            points[ptr1].left = &points[ptr2];
            ptr1--;
            continue;
        }
        ptr2--;
    }
    while (ptr1 >= 0) {
        points[ptr1].left = nullptr;
        ptr1--;
    }
}

SAT2 map_to_2SAT(Pair *pairs, int n, double d) {
//2*n points, 2-SAT vars = {Vp, dim0Lp, dim0Rp, dim1Lp, dim1Rp ,...} -> |vars| = 2n * (1 + 2*dim)
    SAT2 sat2;
    int dim = pairs[0].p.dim;
    sat2.n = 2 * n * (1 + 2 * dim);
    sat2.m = 12 * dim * n + n;
    sat2.a = new int[sat2.m];
    sat2.b = new int[sat2.m];
    for (int i = 0; i < n; ++i) {
        sat2.a[i] = pairs[i].p.id;
        sat2.b[i] = pairs[i].q.id;
    }
    for (int i = 0; i < dim; ++i) {
        left(sorted_points[i], 2 * n, i, d);
        right(sorted_points[i], 2 * n, i, d);
        for (int j = 0; j < 2 * n; ++j) {
            //~Lpv~Vp
            sat2.a[j + n + 12 * i * n] = -(sorted_points[i][j].id + i * 4 * n + 2 * n);
            sat2.b[j + n + 12 * i * n] = -sorted_points[i][j].id;
            //~LpvLl(p)
            sat2.a[j + n + 12 * i * n + 2 * n] = -(sorted_points[i][j].id + i * 4 * n + 2 * n);
            sat2.b[j + n + 12 * i * n + 2 * n] =
                    j - 1 >= 0 ? sorted_points[i][j - 1].id + i * 4 * n + 2 * n : sorted_points[i][j].id + i * 4 * n +
                                                                                  2 * n;
            //~VpvLl(d,p)
            sat2.a[j + n + 12 * i * n + 4 * n] = -sorted_points[i][j].id;
            sat2.b[j + n + 12 * i * n + 4 * n] =
                    sorted_points[i][j].left == nullptr ? sorted_points[i][j].id : (sorted_points[i][j].left->id +
                                                                                    i * 4 * n + 2 * n);
            //~Rpv~Vp
            sat2.a[j + n + 12 * i * n + 6 * n] = -(sorted_points[i][j].id + i * 4 * n + 4 * n);
            sat2.b[j + n + 12 * i * n + 6 * n] = -sorted_points[i][j].id;
            //~RpvRr(p)
            sat2.a[j + n + 12 * i * n + 8 * n] = -(sorted_points[i][j].id + i * 4 * n + 4 * n);
            sat2.b[j + n + 12 * i * n + 8 * n] =
                    j + 1 < 2 * n ? sorted_points[i][j + 1].id + i * 4 * n + 4 * n : sorted_points[i][j].id +
                                                                                     i * 4 * n + 4 * n;
            //~VpvRr(d,p)
            sat2.a[j + n + 12 * i * n + 10 * n] = -sorted_points[i][j].id;
            sat2.b[j + n + 12 * i * n + 10 * n] =
                    sorted_points[i][j].right == nullptr ? sorted_points[i][j].id : sorted_points[i][j].right->id +
                                                                                    i * 4 * n + 4 * n;
        }
    }
//    for (int i = 0; i < sat2.m; ++i) {
//        cout << sat2.a[i] << " " << sat2.b[i] << endl;
//    }
    return sat2;
}

int mat_n;
int mat_idx;

double implicit_matrix(long i, long j) {
    if (i >= mat_n || j >= mat_n)
        return INT_MAX;
    if (i > mat_n - j - 1)
        return sorted_points[mat_idx][i].p[mat_idx] - sorted_points[mat_idx][mat_n - j - 1].p[mat_idx];
    return 0;
}


double binary_search(Pair *pairs, int n, int rank_s, int rank_e) {
    int rank_m = (rank_e + rank_s) / 2;
    if (rank_m == rank_s) {
        double d = select(&implicit_matrix, 2 * n, 2 * n, rank_m + 1);
        SAT2 sat2 = map_to_2SAT(pairs, n, d);
        bool satisfied = satisfiable(sat2.a, sat2.b, sat2.n, sat2.m);
        delete[] sat2.a;
        delete[] sat2.b;
        if (satisfied) return d;
        d = select(&implicit_matrix, 2 * n, 2 * n, rank_e + 1);
        sat2 = map_to_2SAT(pairs, n, d);
        satisfied = satisfiable(sat2.a, sat2.b, sat2.n, sat2.m);
        delete[] sat2.a;
        delete[] sat2.b;
        if (satisfied) return d;
        return -1;
    }
    double d = select(&implicit_matrix, 2 * n, 2 * n, rank_m + 1);
//    cout << d << endl;
    SAT2 sat2 = map_to_2SAT(pairs, n, d);
    bool satisfied = satisfiable(sat2.a, sat2.b, sat2.n, sat2.m);
//    cout << satisfied << endl;
    delete[] sat2.a;
    delete[] sat2.b;
    if (satisfied)
        return binary_search(pairs, n, rank_s, rank_m);
    return binary_search(pairs, n, rank_m, rank_e);
}


int idx;

bool compare(Point a, Point b) {
    return a.p[idx] < b.p[idx];
}


double smallest_square(Pair *pairs, int n) {
    int dim = pairs[0].p.dim;
    Point *points = new Point[2 * n];
    for (int i = 0; i < n; ++i) {
        points[2 * i] = pairs[i].p;
        points[2 * i + 1] = pairs[i].q;
    }
    sorted_points = new Point *[dim];
    for (int i = 0; i < dim; ++i) {
        sorted_points[i] = new Point[2 * n];
        copy(points, points + 2 * n, sorted_points[i]);
    }
    for (int i = 0; i < dim; ++i) {
        idx = i;
        sort(sorted_points[i], sorted_points[i] + 2 * n, compare);
    }
    double best = -1;
    for (int i = 0; i < dim; ++i) {
        mat_idx = i;
        mat_n = 2 * n;
        double smallest = binary_search(pairs, n, 0, 4 * n * n - 1);
        if (smallest == -1)
            continue;
        if (smallest < best || best == -1)
            best = smallest;
    }
    for (int i = 0; i < dim; ++i) {
        delete[] sorted_points[i];
    }
    delete[] sorted_points;
    delete[] points;
    return best;
}