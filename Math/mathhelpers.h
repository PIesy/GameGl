#ifndef MATHHELPERS_H
#define MATHHELPERS_H

#include "mathdefines.h"
#include "mathconstants.h"
#include <cmath>

constexpr float degToRad(float degrees)
{
    return degrees / 180 * pi;
}

void rotateQuat(glm::fquat& base, float degree, const Vec3& axis);
Mat3 rotationMatrix(const Vec3& axis, float angle);
Mat4 orthographicProjectionMatrix(const Vec4& bounds, float near, float far);

class PerspectiveMatrix
{
    Mat4 matrix;
public:
    PerspectiveMatrix(float aspectRatio, float vFov, float near, float far);
    operator Mat4&();
};

#endif // MATHHELPERS_H

