#ifndef RENDERPATH_H
#define RENDERPATH_H

#include "renderstep.h"
#include <list>

struct RenderPath
{
    std::vector<RenderStep> steps;
};

#endif // RENDERPATH_H
