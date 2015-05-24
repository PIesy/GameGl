#include "mathhelpers.h"

PerspectiveMatrix::PerspectiveMatrix(float scaleX, float scaleY, float near, float far)
{
    matrix.matrix[0][0] = scaleX;
    matrix.matrix[1][1] = scaleY;
    matrix.matrix[2][2] = (near + far) / (near - far);
    matrix.matrix[2][3] = (far * near * 2.0f) / (near - far);
    matrix.matrix[3][2] = -1;
}

void PerspectiveMatrix::setScaleX(float value)
{
    matrix.matrix[0][0] = value;
}

void PerspectiveMatrix::setScaleY(float value)
{
    matrix.matrix[1][1] = value;
}
