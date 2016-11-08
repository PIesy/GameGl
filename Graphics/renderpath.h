#ifndef RENDERPATH_H
#define RENDERPATH_H

#include "renderstep.h"
#include <list>

struct RenderPath
{
    std::list<RenderStep> steps;
};

#endif // RENDERPATH_H
