#include "pick.h"
#include <bits/stdc++.h>

using namespace std;


double pick(double *S, int n, int i) {
    bool first = true;
    while (true) {
        int c = 21;
        int b = n / (2 * c) + 1;
        int d = c / 2;
        int cols = n / c + 1;
        for (int j = 0; j < cols; ++j) {
            sort(S + j * c, S + min(n, (j + 1) * c));
        }
        double m;
        if (cols != 1) {
            double *T = new double[cols];
            for (int j = 0; j < cols - 1; ++j) {
                T[j] = S[j * c + d];
            }
            T[cols - 1] = (cols - 1) * c + d < n ? S[(cols - 1) * c + d] : numeric_limits<double>::max();
            m = pick(T, cols, b);
            delete[] T;
        } else {
            m = S[i - 1];
            if (!first)
                delete[] S;
            return m;
        }
        int rank_m = 1;
        int eq = -1;
        double eps = 0.00000001;
        for (int j = 0; j < n; ++j) {
            if (m - S[j] > eps)
                rank_m++;
            if (abs(m - S[j]) < eps)
                eq++;
        }
        if (rank_m + eq >= i && rank_m <= i) {
            if(!first)
                delete[] S;
            return m;
        }
        double *D;
        if (rank_m > i) {
            D = new double[rank_m - 1];
            int idx = 0;
            for (int j = 0; j < n; ++j) {
                if (S[j] < m - eps)
                    D[idx++] = S[j];
            }
            n = rank_m - 1;
        } else {
            D = new double[n - rank_m - eq];
            int idx = 0;
            for (int j = 0; j < n; ++j) {
                if (S[j] - m > eps)
                    D[idx++] = S[j];
            }
            n = n - rank_m - eq;
            i = i - rank_m - eq;
        }
        if (!first)
            delete[] S;
        S = D;
        first = false;
    }
}
