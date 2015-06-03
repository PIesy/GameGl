#ifndef MATHCONSTANTS
#define MATHCONSTANTS

#include "mathdefines.h"
#include "mathhelpers.h"

const Mat4 identity = {1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1};

const Mat4 perspective = PerspectiveMatrix(1, 1, 0.5, 5);

const glm::fquat rotation = glm::fquat(1, 0, 0, 0);

const float pi = 3.14f;

const Vec4 light = { 0.3, 0.5, 0, 0 };


#endif // MATHCONSTANTS

