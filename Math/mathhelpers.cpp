#include "mathhelpers.h"
#include "mathconstants.h"

void rotateQuat(glm::fquat& base, float degree, const Vec3& axis)
{
    float radian = degToRad(degree);
    float scalar = std::cos(radian / 2.0f);
    Vec3 ax = glm::normalize(axis);
    ax = ax * (float)sin(radian / 2.0f);
    glm::fquat rot(scalar, ax.x, ax.y, ax.z);
    base = rot * base;
}

Mat3 rotationMatrix(const Vec3& axis, float angle)
{
    Mat3 result;
    float rad = degToRad(angle);
    float x = axis.x, y = axis.y, z = axis.z;
    float s = sinf(rad), c = cosf(rad);

    result[0] = {c + powf(x, 2) * (1 - c),
                 x * y * (1 - c) - z * s,
                 x * z * (1 - c) + y * s};

    result[1] = {y * x * (1 - c) + z * s,
                 c + powf(y, 2) * (1 - c),
                 y * z * (1 - c) - x * s};

    result[2] = {z * x * (1 - c) - y * s,
                 z * y * (1 - c) + x * s,
                 c + powf(z, 2) * (1 - c)};
    result = glm::transpose(result);
    return result;
}

Mat4 orthographicProjectionMatrix(const Vec4& bounds, float near, float far)
{
    Mat4 result(1.0f);

    result[0][0] = 2.0f / (bounds[1] - bounds[0]);
    result[1][1] = 2.0f / (bounds[3] - bounds[2]);
    result[2][2] = -2.0f / (far - near);
    result[3] = {-(bounds[1] + bounds[0]) / (bounds[1] - bounds[0]),
                 -(bounds[3] + bounds[2]) / (bounds[3] - bounds[2]),
                 -(far + near) / (far - near),
                 1};
    return result;
}

PerspectiveMatrix::PerspectiveMatrix(float aspectRatio, float vFov, float near, float far)
{
    matrix[0][0] = 1.0f / (aspectRatio * std::tan(degToRad(vFov / 2)));
    matrix[1][1] = 1 / std::tan(degToRad(vFov / 2));
    matrix[2][2] = (near + far) / (near - far);
    matrix[3][2] = (far * near * 2.0f) / (near - far);
    matrix[2][3] = -1;
}

PerspectiveMatrix::operator Mat4&()
{
    return matrix;
}
