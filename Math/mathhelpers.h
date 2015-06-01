#ifndef MATHHELPERS_H
#define MATHHELPERS_H

#include "mathdefines.h"

float degToRad(float degrees);

class PerspectiveMatrix: public Mat4
{
public:
    PerspectiveMatrix(float scaleX, float scaleY, float near, float far);
    void setScaleX(float value);
    void setScaleY(float value);
};

class RotationMatrix: public Mat4
{
    void setValues(float x, float y, float z, float degree);
public:
    enum class Axis {X, Y, Z};
    RotationMatrix(float degree, Axis axis = Axis::Z);
};

#endif // MATHHELPERS_H

