#ifndef RENDERSTEP_H
#define RENDERSTEP_H

#include "graphicsclasses.h"
#include "rendertarget.h"
#include "rendering_attributtes.h"
#include <list>

struct RenderStep
{
    std::list<GraphicsObject> objects;
    std::list<RenderTarget> targets;
    RenderTarget target = ScreenRenderTarget();
    RenderingAttributes attributes;
    Program* prog;
    ConfigFunction preConfig = defaultConfig;
    ConfigFunction postConfig = defaultConfig;
};

#endif // RENDERSTEP_H
