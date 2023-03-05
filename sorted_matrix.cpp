#include "sorted_matrix.h"
#include <vector>
#include <cmath>
#include <bits/stdc++.h>


Cell *split_cells(Cell *cells, int n) {
    Cell *n_cells = new Cell[4 * n];
    for (int idx = 0; idx < n; ++idx) {
        Cell cell = cells[idx];
        long i = cell.i;
        long j = cell.j;
        if (cell.h == 1) {
            n_cells[4 * idx] = Cell{i, j, cell.w / 4, 1};
            n_cells[4 * idx + 1] = Cell{i, j + cell.w / 4, cell.w / 4, 1};
            n_cells[4 * idx + 2] = Cell{i, j + cell.w / 2, cell.w / 4, 1};
            n_cells[4 * idx + 3] = Cell{i, j + 3 * cell.w / 4, cell.w / 4, 1};
        } else if (cell.w == 1) {
            n_cells[4 * idx] = Cell{i, j, 1, cell.h / 4};
            n_cells[4 * idx + 1] = Cell{i + cell.h / 4, j, 1, cell.h / 4};
            n_cells[4 * idx + 2] = Cell{i + cell.h / 2, j, 1, cell.h / 4};
            n_cells[4 * idx + 3] = Cell{i + 3 * cell.h / 4, j, 1, cell.h / 4};
        } else {
            n_cells[4 * idx] = Cell{i, j, cell.w / 2, cell.h / 2};
            n_cells[4 * idx + 1] = Cell{i + cell.h / 2, j, cell.w / 2, cell.h / 2};
            n_cells[4 * idx + 2] = Cell{i + cell.h / 2, j + cell.w / 2, cell.w / 2, cell.h / 2};
            n_cells[4 * idx + 3] = Cell{i, j + cell.w / 2, cell.w / 2, cell.h / 2};
        }
    }
    delete[]cells;
    return n_cells;
}

Cell select(double (*matrix)(long, long), int n, int m, long k) {
    double eps = 0.00001;
    long mm = pow(2, ceil(log2(m)));
    long nn = pow(2, ceil(log2(n)));
    if ((int) (log2(mm) + log2(nn)) % 2)
        nn *= 2;
    long S = nn * mm;
    Cell *cells = new Cell[1];
    cells[0] = Cell{0, 0, mm, nn};
    int cell_count = 1;
    for (long p = 1; p < (long) (log(S) / log(4)) + 1; ++p) {
        cells = split_cells(cells, cell_count);
        cell_count *= 4;
        long Bp = mm < (1 << (p + 1)) - 1 ? mm : (1 << (p + 1)) - 1;
        long q = ceil(pow(4, p) * k / S);
        q += Bp;
        if (q <= cell_count) {
            double *mins = new double[cell_count];
            for (long i = 0; i < cell_count; ++i) {
                mins[i] = matrix(cells[i].i, cells[i].j);
            }
            sort(mins, mins + cell_count);
            double Xu = mins[q - 1];
            Cell *n_cells = new Cell[q - 1];
            int idx = 0;
            for (int i = 0; i < cell_count; i++) {
                if (matrix(cells[i].i, cells[i].j) < Xu)
                    n_cells[idx++] = cells[i];
            }
            int i = 0;
            while (idx != q - 1) {
                if (abs(matrix(cells[i].i, cells[i].j) - Xu) < eps)
                    n_cells[idx++] = cells[i];
                i++;
            }
            delete[] cells;
            cells = n_cells;
            cell_count = q - 1;
            delete[] mins;
        }
        long r = floor(k * pow(4, p) / S) - Bp;
        if (r >= 1) {
            double *maxs = new double[cell_count];
            for (long i = 0; i < cell_count; ++i) {
                maxs[i] = matrix(cells[i].i + cells[i].h - 1, cells[i].j + cells[i].w - 1);
            }
            sort(maxs, maxs + cell_count);
            double Xl = maxs[r - 1];
            Cell *n_cells = new Cell[cell_count - r];
            int idx = 0;
            for (int i = 0; i < cell_count; i++) {
                if (matrix(cells[i].i + cells[i].h - 1, cells[i].j + cells[i].w - 1) > Xl)
                    n_cells[idx++] = cells[i];
            }
            int i = 0;
            while (idx != cell_count - r) {
                Cell cell = cells[i];
                if (abs(matrix(cell.i + cell.h - 1, cell.j + cell.w - 1) - Xl) < eps) {
                    n_cells[idx++] = cells[i];
                }
                i++;
            }
            k = k - r * (S / (1 << (2 * p)));
            delete[]cells;
            cells = n_cells;
            cell_count -= r;
            delete[]maxs;
        }
    }
    sort(cells, cells + cell_count, [matrix](const Cell &a, const Cell &b) -> bool {
        return matrix(a.i, a.j) < matrix(b.i, b.j);
    });
    Cell cell = cells[k - 1];
    delete[] cells;
    return cell;
}