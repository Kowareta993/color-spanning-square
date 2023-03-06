#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <bits/stdc++.h>
#include "SAT2.h"
#include "square.h"
#include "sorted_matrix.h"
#include "pick.h"


using namespace std;
using namespace std::chrono;

void SAT2_validation(int);

void SAT2_timing(int);

void Square_validation(int);

void Square_timing(int);

void Select_validation(int);

void Select_timing(int);

void Pick_validation(int);

void Pick_timing(int);

Pair *randomPairs(int, int);

void save(Pair *, int);

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    if (argc == 1) {
        Pick_validation(1000);
        SAT2_validation(1000);
        Select_validation(1000);
        Square_validation(1000);
        Pick_timing(10);
        SAT2_timing(10);
        Select_timing(10);
        Square_timing(10);
        return 0;
    }
    int n, dim;
    if (argc != 3)
        return -1;
    n = atoi(argv[1]);
    dim = atoi(argv[2]);
    Pair *pairs = randomPairs(n, dim);
    save(pairs, n);
    double result = smallest_square(pairs, n);
    delete[] pairs;
    cout << result << endl;
}

void save(Pair *pairs, int n) {
    ofstream stream("result/points.csv");
    int dim = pairs[0].p.dim;
    for (int i = 0; i < dim; ++i) {
        stream << "dim" << i << ",";
    }
    stream << "color" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < dim; ++j) {
            stream << pairs[i].p.p[j] << ",";
        }
        stream << pairs[i].p.color << endl;
        for (int j = 0; j < dim; ++j) {
            stream << pairs[i].q.p[j] << ",";
        }
        stream << pairs[i].q.color << endl;
    }
    stream.close();
}

int *randomIntArr(int size, int min, int max) {
    int *arr = new int[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % (max - min + 1) + min;
    }
    return arr;
}

double *randomDoubleArr(int n, double min, double max) {
    random_device rd;
    mt19937 e2(rd());
    uniform_real_distribution<> dist(min, max);
    double *arr = new double[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = dist(e2);
    }
    return arr;
}

void SAT2_timing(int trials) {
    ofstream stream("analysis/SAT2_timing.csv");
    stream << 'n' << "," << 't' << endl;
    for (int t = 10; t < 1000 * 1000; t *= 10) {
        for (int n = t; n < t * 10; n += t) {
            long x = 0;
            for (int i = 0; i < trials; ++i) {
                int m = n;
                int *a = randomIntArr(m, -n, n);
                int *b = randomIntArr(m, -n, n);
                for (int j = 0; j < m; ++j) {
                    if (a[j] == 0)
                        a[j] = 1;
                    if (b[j] == 0)
                        b[j] = 1;
                }
                auto start = high_resolution_clock::now();
                satisfiable(a, b, n, m);
                auto stop = high_resolution_clock::now();
                x += duration_cast<microseconds>(stop - start).count();
                delete[] a;
                delete[] b;
            }
            stream << n << "," << x / trials << endl;
        }
    }
    stream.close();
    cout << "SAT-2 timing finished" << endl;
}

bool SAT2_bf(int *a, int *b, int n, int m, bool *vars, int step) {
    if (step == n) {
        for (int i = 0; i < m; ++i) {
            bool x = a[i] > 0 ? vars[a[i] - 1] : !vars[-a[i] - 1];
            bool y = b[i] > 0 ? vars[b[i] - 1] : !vars[-b[i] - 1];
            if (!(x | y))
                return false;
        }
        return true;
    }
    if (SAT2_bf(a, b, n, m, vars, step + 1))
        return true;
    vars[step] = true;
    if (SAT2_bf(a, b, n, m, vars, step + 1))
        return true;
    vars[step] = false;
    return false;
}

void SAT2_validation(int trials) {
    for (int i = 0; i < trials; ++i) {
        int n = rand() % 20 + 1;
        int m = rand() % 20 + 1;
        int *a = randomIntArr(m, -n, n);
        int *b = randomIntArr(m, -n, n);
        for (int j = 0; j < m; ++j) {
            if (a[j] == 0)
                a[j] = 1;
            if (b[j] == 0)
                b[j] = 1;
        }
        bool x1 = satisfiable(a, b, n, m);
        bool *vars = new bool[n];
        for (int j = 0; j < n; ++j) {
            vars[j] = false;
        }
        bool x2 = SAT2_bf(a, b, n, m, vars, 0);
        delete[] vars;
        if (x1 != x2) {
            cout << "SAT-2 Validation failed" << endl;
            cout << x1 << x2 << endl;
            for (int j = 0; j < m; ++j) {
                cout << a[j] << "v" << b[j] << "^";
            }
            cout << endl;
            delete[] a;
            delete[] b;
            return;
        }
        delete[] a;
        delete[] b;
    }
    cout << "SAT-2 Validation passed" << endl;
}

double Square_bf(Pair *pairs, int n, vector<Point> &points, int step, int dim) {
    if (step == n) {
        double d = 0;
        for (int i = 0; i < dim; ++i) {
            double l = points[0].p[i];
            double h = points[0].p[i];
            for (int j = 0; j < n; ++j) {
                l = min(l, points[j].p[i]);
                h = max(h, points[j].p[i]);
            }
            d = max(d, h - l);
        }
        return d;
    }
    vector<Point> n_points(points);
    n_points.push_back(pairs[step].p);
    double d = Square_bf(pairs, n, n_points, step + 1, dim);
    vector<Point> nn_points(points);
    nn_points.push_back(pairs[step].q);
    return min(d, Square_bf(pairs, n, nn_points, step + 1, dim));
}

Pair *randomPairs(int n, int dim) {
    Pair *pairs = new Pair[n];
    for (int k = 0; k < n; ++k) {
        pairs[k].p.dim = dim;
        pairs[k].q.dim = dim;
        pairs[k].p.color = k;
        pairs[k].q.color = k;
        pairs[k].p.id = 2 * k + 1;
        pairs[k].q.id = 2 * k + 2;
        pairs[k].p.p = new double[dim];
        pairs[k].q.p = new double[dim];
    }
    for (int j = 0; j < dim; ++j) {
        double *arr = randomDoubleArr(2*n, -100, 100);
        for (int k = 0; k < n; ++k) {
            pairs[k].p.p[j] = arr[2*k];
            pairs[k].q.p[j] = arr[2*k + 1];
        }
        delete[] arr;
    }
    return pairs;
}

void Square_validation(int trials) {
    int n = 10;
    int dim = 3;
    for (int i = 0; i < trials; ++i) {
        Pair *pairs = randomPairs(n, dim);
        vector<Point> p;
        double d1 = Square_bf(pairs, n, p, 0, dim);
        double d2 = smallest_square(pairs, n);
        if (abs(d1 - d2) > 0.0001) {
            cout << "Square Validation failed" << endl;
            cout << d1 << " " << d2 << endl;
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < dim; ++k) {
                    cout << pairs[j].p.p[k] << " ";
                }
                cout << ",";
                for (int k = 0; k < dim; ++k) {
                    cout << pairs[j].q.p[k] << " ";
                }
                cout << endl;
            }
            for (int j = 0; j < n; ++j) {
                delete[] pairs[j].p.p;
                delete[] pairs[j].q.p;
            }
            delete[] pairs;
            return;
        }
        for (int j = 0; j < n; ++j) {
            delete[] pairs[j].p.p;
            delete[] pairs[j].q.p;
        }
        delete[] pairs;
    }
    cout << "Square Validation passed" << endl;
}

void Square_timing(int trials) {
    ofstream stream("analysis/Square_timing.csv");
    stream << "n,dim,t" << endl;
    for (int t = 10; t < 1000; t *= 10) {
        for (int n = t; n < t * 10; n += t) {
            for (int d = 1; d < 5; ++d) {
                long x = 0;
                for (int i = 0; i < trials; ++i) {
                    Pair *pairs = randomPairs(n, d);
                    auto start = high_resolution_clock::now();
                    smallest_square(pairs, n);
                    auto stop = high_resolution_clock::now();
                    x += duration_cast<microseconds>(stop - start).count();
                    for (int i = 0; i < n; ++i) {
                        delete[]pairs[i].p.p;
                        delete[]pairs[i].q.p;
                    }
                    delete[] pairs;
                }

                stream << n << "," << d << "," << x / trials << endl;
            }

        }
    }
    stream.close();
    cout << "Square timing finished" << endl;
}

int **matrix;
int m, n;

double mat1(long i, long j) {
    if (i >= n || i < 0)
        return INT_MAX;
    if (j >= m || j < 0)
        return INT_MAX;
    return matrix[i][j];
}

void random_sorted_matrix(int n, int m) {
    matrix = new int *[n];
    for (int j = 0; j < n; ++j) {
        matrix[j] = randomIntArr(m, -100, 100);
        sort(matrix[j], matrix[j] + m);
    }
    for (int j = 0; j < m; ++j) {
        int *col = new int[n];
        for (int k = 0; k < n; ++k) {
            col[k] = matrix[k][j];
        }
        sort(col, col + n);
        for (int k = 0; k < n; ++k) {
            matrix[k][j] = col[k];
        }
        delete[] col;
    }


}

void Select_validation(int trials) {
    for (int i = 0; i < trials; ++i) {
        m = rand() % 100 + 1;
        n = rand() % 100 + 1;
        random_sorted_matrix(n, m);
        int k = rand() % (n * m);
        double a = select(&mat1, n, m, k + 1);
        double *mat = new double[n * m];
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < m; ++k) {
                mat[j * m + k] = matrix[j][k];
            }
        }
        sort(mat, mat + n * m);
        double b = mat[k];
        delete[] mat;
        if (abs(b - a) > 0.0001) {
            cout << "Select Validation failed" << endl;
            cout << a << " " << b << endl;
            cout << k << endl;
            for (int j = 0; j < n; ++j) {
                for (int l = 0; l < m; ++l) {
                    cout << matrix[j][l] << " ";
                }
                cout << endl;
            }
            for (int j = 0; j < n; ++j) {
                delete[] matrix[j];

            }
            delete[] matrix;

            return;
        }
        for (int j = 0; j < n; ++j) {
            delete[] matrix[j];

        }
        delete[] matrix;
    }


    cout << "Select Validation passed" << endl;
}

double mat2(long i, long j) {
    return i + j;
}

void Select_timing(int trials) {
    ofstream stream("analysis/Select_timing.csv");
    stream << "n,t" << endl;
    for (int t = 10; t < 1000 * 1000; t *= 10) {
        for (long n = t; n < t * 10; n += t) {
            long x = 0;
            for (int i = 0; i < trials; ++i) {
                long k = rand() % min(n * n, 1000L);
                auto start = high_resolution_clock::now();
                select(&mat2, n, n, k + 1);
                auto stop = high_resolution_clock::now();
                x += duration_cast<microseconds>(stop - start).count();
            }

            stream << n << "," << x / trials << endl;
        }
    }
    stream.close();
    cout << "Select timing finished" << endl;
}


void Pick_validation(int trials) {
    for (int i = 0; i < trials; ++i) {
        int n = rand() % 1000 + 1;
        double *arr = randomDoubleArr(n, -5, 5);
        int k = rand() % (n);
        double a = pick(arr, n, k + 1);
        sort(arr, arr + n);
        double b = arr[k];
        if (abs(b - a) > 0.0001) {
            cout << "Pick Validation failed" << endl;
            cout << a << " " << b << endl;
            cout << k << endl;
            for (int j = 0; j < n; ++j) {
                cout << arr[j] << " ";
            }
            cout << endl;
            delete[] arr;
            return;
        }
        delete[] arr;
    }
    cout << "Pick Validation passed" << endl;
}

void Pick_timing(int trials) {
    ofstream stream("analysis/Pick_timing.csv");
    stream << "n,t" << endl;
    for (int t = 10; t < 1000 * 1000; t *= 10) {
        for (int n = t; n < t * 10; n += t) {
            long x = 0;
            for (int i = 0; i < trials; ++i) {
                int k = rand() % n + 1;
                double *arr = randomDoubleArr(n, -100, 100);
                auto start = high_resolution_clock::now();
                pick(arr, n, k);
                auto stop = high_resolution_clock::now();
                x += duration_cast<microseconds>(stop - start).count();
            }
            stream << n << "," << x / trials << endl;
        }
    }
    stream.close();
    cout << "Pick timing finished" << endl;
}