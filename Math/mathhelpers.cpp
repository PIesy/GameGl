#include "mathhelpers.h"
#include "mathconstants.h"
#include <cmath>
#include "Logger/logger.h"

float degToRad(float degrees)
{
    return degrees / 180 * pi;
}

void rotateQuat(glm::fquat& base, float degree, const Vec3& axis)
{
    float radian = degToRad(degree);
    float scalar = cos(radian / 2.0f);
    Vec3 ax = glm::normalize(axis);
    ax = ax * (float)sin(radian / 2.0f);
    glm::fquat rot(scalar, ax.x, ax.y, ax.z);
    base = rot * base;
}

PerspectiveMatrix::PerspectiveMatrix(float scaleX, float scaleY, float near, float far)
{
    matrix[0][0] = scaleX;
    matrix[1][1] = scaleY;
    matrix[2][2] = (near + far) / (near - far);
    matrix[2][3] = (far * near * 2.0f) / (near - far);
    matrix[3][2] = -1;
}

PerspectiveMatrix::operator Mat4&()
{
    return matrix;
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

RotationMatrix::operator Mat4&()
{
    return matrix;
}

void RotationMatrix::setValues(float x, float y, float z, float degree)
{
    matrix[0][0] = x * x + (1 - x * x) * cos(degree);
    matrix[0][1] = (1 - cos(degree)) * x * y - z * sin(degree);
    matrix[0][2] = (1 - cos(degree)) * x * z + y * sin(degree);
    matrix[1][0] = (1 - cos(degree)) * x * y + z * sin(degree);
    matrix[1][1] = y * y + (1 - y * y) * cos(degree);
    matrix[1][2] = (1 - cos(degree)) * y * z - x * sin(degree);
    matrix[2][0] = (1 - cos(degree)) * x * z - y * sin(degree);
    matrix[2][1] = (1 - cos(degree)) * y * z + x * sin(degree);
    matrix[2][2] = z * z + (1 - z * z) * cos(degree);
    matrix[3][3] = 1;
}
