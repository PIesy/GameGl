#ifndef MATHHELPERS_H
#define MATHHELPERS_H

#include "mathdefines.h"

class PerspectiveMatrix: public Mat4
{
public:
    PerspectiveMatrix(float scaleX, float scaleY, float near, float far);
    void setScaleX(float value);
    void setScaleY(float value);
};

#endif // MATHHELPERS_H

