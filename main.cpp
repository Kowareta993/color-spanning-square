#include <iostream>
#include "SAT2.h"

using namespace std;


int main()
{
    int n, m;
    cin >> n >> m;
    int *a = new int[m];
    int *b = new int[m];
    for (int i = 0; i < m; ++i) {
        cin >> a[i] >> b[i];
    }
    cout << satisfiable(a, b, n, m) << endl;
}