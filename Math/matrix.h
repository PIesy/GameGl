#ifndef MATRIX_H
#define MATRIX_H

#include "Helpers/size.h"
#include <utility>
#include <array>

template<int i, int j, class T = float>
struct BasicMatrix
{
    T matrix[i][j] = {{0}};

    BasicMatrix() {}
    BasicMatrix(const std::initializer_list<T>& initializer)
    {
        int x = 0;

        for(const T& value: initializer)
        {
            matrix[x / i][x % i] = value;
            x++;
            if(x == i * j)
                break;
        }
    }
};

template<int i, int j, class T = float>
class Matrix
{
protected:
    BasicMatrix<i, j, T> matrix;
public:
    Matrix(const std::initializer_list<T> initializer):matrix(initializer) {}

    Matrix(BasicMatrix<i, j, T> source)
    {
        matrix = source;
    }

    Matrix() {}

    T* getData()
    {
        return matrix.matrix[0];
    }

    T* operator[](int index)
    {
        return matrix.matrix[index];
    }

    Matrix<j, i> Transpose() const
    {
        Matrix<i, j> result;

        for(int i1 = 0; i1 < j; i1++)
            for(int j1 = 0; j1 < i; j1++)
                result[i1][j1] = matrix[j1][i1];
        return result;
    }

    Size getSize()
    {
        return {i, j};
    }
};

template<int i, class T>
class Matrix<i, 1, T>
{
protected:
    BasicMatrix<i, 1, T> matrix;
public:
    Matrix(const std::initializer_list<T> initializer):matrix(initializer) {}

    Matrix(BasicMatrix<i, 1, T> source)
    {
        matrix = source;
    }

    Matrix() {}

    T* getData()
    {
        return matrix.matrix[0];
    }

    const T* getData() const
    {
        return matrix.matrix[0];
    }

    T& operator[](int index)
    {
        return matrix.matrix[index][0];
    }

    const T& operator[](int index) const
    {
        return matrix.matrix[index][0];
    }

    Matrix<1, i> Transpose() const
    {
        Matrix<1, i> result;

        for(int j1 = 0; j1 < i; j1++)
            result[0][j1] = matrix[j1][0];
        return result;
    }

    Size getSize()
    {
        return {i, 1};
    }
};

template<int i1, int j1, int j2, class T>
Matrix<i1, j2> operator*(const Matrix<i1, j1, T>& first, const Matrix<j1, j2, T>& second)
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
Matrix<i1, j1> operator+(const Matrix<i1, j1, T>& first, const Matrix<i1, j1, T>& second)
{
    Matrix<i1, j1> result;

    for(int i = 0; i < i1; i++)
        for(int j = 0; j < j1; j++)
            result[i][j] = first[i][j] + second[i][j];
    return result;
}

#endif // MATRIX_H

