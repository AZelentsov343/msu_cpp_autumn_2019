
#include <iostream>
#include "Matrix.h"

using namespace std;

int main() {
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m = Matrix(rows, cols);

    if (m.getRows() != 5) {
        cerr << "Wrong get rows!" << endl;
        return 1;
    }
    if (m.getColumns() != 3) {
        cerr << "Wrong get columns!" << endl;
        return 1;
    }

    m[1][2] = 5; // строка 1, колонка 2
    if (m[1][2] != 5) {
        cerr << "Cannot write to a matrix!" << endl;
        return 1;
    }
    double x = m[4][1];
    if (x != 0.0) {
        cerr << "Cannot read from a matrix!" << endl;
        return 1;
    }

    m *= 3;
    if (m[1][2] != 15) {
        cerr << "Cannot multiply matrix!" << endl;
        return 1;
    }

    bool caught = true;

    try {
        m[10][1] = 5;
        caught = false;
    } catch (out_of_range &) {
        caught = true;
    }

    if (!caught) {
        cerr << "Do not throw exception in case row out of range" << endl;
        return 1;
    }

    try {
        m[1][10] = 5;
        caught = false;
    } catch (out_of_range &) {
        caught = true;
    }

    if (!caught) {
        cerr << "Do not throw exception in case col out of range when we write to matr" << endl;
        return 1;
    }

    try {
        int x = m[1][10];
        caught = false;
    } catch (out_of_range &) {
        caught = true;
    }

    if (!caught) {
        cerr << "Do not throw exception in case column out of range when we read from matr" << endl;
        return 1;
    }

    Matrix m1 = Matrix(rows, cols);
    Matrix m2 = Matrix(rows, cols);

    if (m1 != m2) {
        cerr << "Wrong use != for equal matrix" << endl;
        return 1;
    }

    if (m1 == m2) {

    } else {
        cerr << "Wrong use == for equal matrix" << endl;
        return 1;
    }

    m2[1][1] = 1;

    if (m1 == m2) {
        cerr << "Wrong use == for non-equal matrix" << endl;
        return 1;
    }

    if (m1 != m2) {

    } else {
        cerr << "Wrong use != for non-equal matrix" << endl;
        return 1;
    }

    Matrix m3 = Matrix(rows + 1, cols);
    if (m1 == m3) {
        cerr << "Wrong use == for matrix of different shapes" << endl;
        return 1;
    }

    cout << "Everything OK" << endl;
    return 0;
}