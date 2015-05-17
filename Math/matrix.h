#ifndef MATRIX_H
#define MATRIX_H

template<int i, int j, class T = float>
class Matrix
{
    T matrix[i][j] = {0};
public:
    T* getData() const
    {
        return matrix;
    }

    T* operator[](int i) const
    {
        return matrix[i];
    }

    Matrix<j, i> Transpose() const
    {
        Matrix<j, i> result;

        for(int i1 = 0; i1 < j; i1++)
            for(int j1 = 0; j1 < i; j1++)
                result[i1][j1] = matrix[j1][i1];
        return result;
    }
};

template<int i1, int j1, int j2, class T>
Matrix operator*(const Matrix<i1, j1, T>& first, const Matrix<j1, j2, T>& second)
{
    Matrix<i1, j2> result;
    T tmp;

    for(int i = 0; i < i1; i++)
        for(int j = 0; j < j2; j++)
        {
            tmp = first[i][0] * second[0][j];
            for(int k = 1; k < j1; k++)
                tmp += first[i][k] * second[k][j];
            result[i][j] = tmp;
        }
    return result;
}


template<int i1, int j1, class T>
Matrix operator+(const Matrix<i1, j1, T>& first, const Matrix<i1, j1, T>& second)
{
    Matrix<i1, j1> result;

    for(int i = 0; i < i1; i++)
        for(int j = 0; j < j1; j++)
            result[i][j] = first[i][j] + second[i][j];
    return result;
}

#endif // MATRIX_H

