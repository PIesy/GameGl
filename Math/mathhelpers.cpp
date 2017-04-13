#include "mathhelpers.h"
#include "mathconstants.h"

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

PerspectiveMatrix::PerspectiveMatrix(float aspectRatio, float vFov, float near, float far)
{
    matrix[0][0] = 1.0f / (aspectRatio * tan(degToRad(vFov / 2)));
    matrix[1][1] = 1 / tan(degToRad(vFov / 2));
    matrix[2][2] = (near + far) / (near - far);
    matrix[3][2] = (far * near * 2.0f) / (near - far);
    matrix[2][3] = -1;
}

PerspectiveMatrix::operator Mat4&()
{
    return matrix;
}
