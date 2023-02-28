#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include "SAT2.h"


using namespace std;
using namespace std::chrono;

void SAT2_validation(int);

void SAT2_timing();

int main() {
    srand(time(nullptr));
    SAT2_validation(1000);
    SAT2_timing();
}

int *random_arr(int size, int min, int max) {
    int *arr = new int[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % (max - min + 1) + min;
    }
    return arr;
}

void SAT2_timing() {
    ofstream stream("analysis/SAT2_timing.csv");
    stream << 'n' << ","  << 't' << endl;
    for (int t = 10; t < 1000 * 1000; t *= 10) {
        for (int n = t; n < t * 10; n += t) {
            int m = n;
            int *a = random_arr(m, -n, n);
            int *b = random_arr(m, -n, n);
            for (int j = 0; j < m; ++j) {
                if (a[j] == 0)
                    a[j] = 1;
                if (b[j] == 0)
                    b[j] = 1;
            }
            auto start = high_resolution_clock::now();
            satisfiable(a, b, n, m);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            free(a);
            free(b);
            stream << n << ","  << duration.count() << endl;
        }
    }
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
        int *a = random_arr(m, -n, n);
        int *b = random_arr(m, -n, n);
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
        free(vars);
        if (x1 != x2) {
            cout << "SAT-2 Validation failed" << endl;
            cout << x1 << x2 << endl;
            for (int j = 0; j < m; ++j) {
                cout << a[j] << "v" << b[j] << "^";
            }
            cout << endl;
            return;
        }
    }
    cout << "SAT-2 Validation passed" << endl;

}