#include <iostream>
#include <cstdlib>
#include "timer.cpp"
#include "sum_by_columns.h"
using namespace std;


void sum_by_columns(int **mas, int n, int m) {
    printf("By columns:\n");
    int i, j;
    int sum = 0;
    Timer t;
    for (j = 0; j < m; j++) {
        for (i = 0; i < n; i++) {
            sum += mas[i][j];
        }
    }
    cout << sum << endl;
}
