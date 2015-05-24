#ifndef MATHCONSTANTS
#define MATHCONSTANTS

#include "mathdefines.h"
#include "mathhelpers.h"

const Mat4 identity = {1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1};

const Mat4 perspective = PerspectiveMatrix(1.0, 1.0, 0.5, 3);


#endif // MATHCONSTANTS

