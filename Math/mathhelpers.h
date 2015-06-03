#ifndef MATHHELPERS_H
#define MATHHELPERS_H

#include "mathdefines.h"
#include <cmath>

float degToRad(float degrees);
void rotateQuat(glm::fquat& base, float degree, const Vec3& axis);

class PerspectiveMatrix
{
    Mat4 matrix;
public:
    PerspectiveMatrix(float scaleX, float scaleY, float near, float far);
    operator Mat4&();
};

class RotationMatrix
{
    Mat4 matrix;
    void setValues(float x, float y, float z, float degree);
public:
    enum class Axis {X, Y, Z};
    RotationMatrix(float degree, Axis axis = Axis::Z);
    operator Mat4&();
};

#endif // MATHHELPERS_H

