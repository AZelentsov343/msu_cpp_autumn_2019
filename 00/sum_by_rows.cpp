#include <iostream>
#include <cstdlib>
#include "timer.cpp"
#include "sum_by_rows.h"

using namespace std;


void sum_by_rows(int **mas, int n, int m) {
    printf("By rows:\n");
    int i, j;
    int sum = 0;
    Timer t;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            sum += mas[i][j];
        }
    }
    cout << sum << endl;
}

