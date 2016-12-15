#ifndef MATHCONSTANTS
#define MATHCONSTANTS

#include "mathdefines.h"
#include "mathhelpers.h"

const Mat4 identity = {1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1};

const Mat4 perspective = PerspectiveMatrix(1, 65.0f, 0.5, 5);

const glm::fquat rotation = glm::fquat(1, 0, 0, 0);

const float pi = 3.14f;

const Vec4 light = { 50, 100, 50, 0 };


#endif // MATHCONSTANTS

