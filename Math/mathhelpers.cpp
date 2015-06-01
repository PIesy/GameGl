#include "mathhelpers.h"
#include "mathconstants.h"
#include <cmath>

float degToRad(float degrees)
{
    return degrees / 180 * pi;
}

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


RotationMatrix::RotationMatrix(float degree, Axis axis)
{
    float radians = degToRad(degree);
    float x = 0, y = 0, z = 0;
    switch(axis)
    {
    case Axis::X:
        x = 1;
        break;
    case Axis::Y:
        y = 1;
        break;
    case Axis::Z:
        z = 1;
        break;
    }
    setValues(x, y, z, radians);
}

void RotationMatrix::setValues(float x, float y, float z, float degree)
{
    matrix.matrix[0][0] = x * x + (1 - x * x) * cos(degree);
    matrix.matrix[0][1] = (1 - cos(degree)) * x * y - z * sin(degree);
    matrix.matrix[0][2] = (1 - cos(degree)) * x * z + y * sin(degree);
    matrix.matrix[1][0] = (1 - cos(degree)) * x * y + z * sin(degree);
    matrix.matrix[1][1] = y * y + (1 - y * y) * cos(degree);
    matrix.matrix[1][2] = (1 - cos(degree)) * y * z - x * sin(degree);
    matrix.matrix[2][0] = (1 - cos(degree)) * x * z - y * sin(degree);
    matrix.matrix[2][1] = (1 - cos(degree)) * y * z + x * sin(degree);
    matrix.matrix[2][2] = z * z + (1 - z * z) * cos(degree);
    matrix.matrix[3][3] = 1;
}
