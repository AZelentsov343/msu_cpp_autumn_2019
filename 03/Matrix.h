#ifndef INC_03_MATRIX_H
#define INC_03_MATRIX_H


class Matrix {
public:
    Matrix(int rows, int columns);

    ~Matrix();

    const Matrix &operator*=(int x);

    bool operator==(const Matrix& m) const;

    bool operator!=(const Matrix& m) const;

    int getRows() const;

    int getColumns() const;

    class Row {
    public:
        Row(int *row, int len);

        const int &operator[](int col) const;

        int &operator[](int col);

    private:
        int length;
        int *row_ptr;
    };

    Row operator[](int row);
    const Row operator[](int row) const;

private:
    int rows;
    int cols;
    int **matr_ptr;
};


#endif //INC_03_MATRIX_H
