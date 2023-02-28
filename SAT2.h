#ifndef SAT2_H
#define SAT2_H


using namespace std;

bool satisfiable(int *a, int *b, int n, int m); //(a[0]vb[0])^(a[1]vb[1])^...(a[m-1]vb[m-1]) for n vars 1..n, var=-i is logical not of i
#endif
