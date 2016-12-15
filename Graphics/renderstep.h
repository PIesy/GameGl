#ifndef RENDERSTEP_H
#define RENDERSTEP_H

#include "graphicsclasses.h"
#include "rendertarget.h"
#include "rendering_attributtes.h"
#include <list>
#include <functional>

struct RenderStep
{
    std::list<int> objects;
    std::list<RenderTarget> targets;
    RenderTarget target = ScreenRenderTarget();
    RenderingAttributes attributes;
    Program* prog;
    Texture genericTexture;
    ConfigFunction preConfig = defaultConfig;
    ConfigFunction postConfig = defaultConfig;
};

#endif // RENDERSTEP_H
