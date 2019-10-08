#include <iostream>
#include <cstdlib>
#include <time.h>
#include "sum_by_columns.h"
#include "sum_by_rows.h"

enum
{
    N = 10000,
    M = 1000
};

int main() {
    int i, j;
    srand(time(NULL));
    int **mas = new int*[N];
    for (i = 0; i < N; i++) {
        mas[i] = new int[M];
        for  (j = 0; j < M; j++) {
            mas[i][j] = rand() % 2;
        }
    }
    sum_by_rows(mas, N, M);
    sum_by_columns(mas, N, M);
    return 0;
}
