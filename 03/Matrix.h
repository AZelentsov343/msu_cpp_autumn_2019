#ifndef INC_03_MATRIX_H
#define INC_03_MATRIX_H


class Matrix {
public:
    Matrix(int rows, int columns);

    ~Matrix();

    Matrix &operator*=(int x);

    bool operator==(const Matrix& m) const;

    bool operator!=(const Matrix& m) const;

    int getRows() const;

    int getColumns() const;

    class Row {
    public:
        Row(int *row, int len);

        const int &operator[](int col) const;

        int &operator[](int col);

        int *row_ptr;
    private:
        int length;
    };

    Row &operator[](int row);
    const Row &operator[](int row) const;

private:
    Row *row;
    int rows;
    int cols;
    int **matr_ptr;
};


#endif //INC_03_MATRIX_H
