#ifndef RENDERSTEP_H
#define RENDERSTEP_H

#include "graphicsclasses.h"
#include "rendertarget.h"
#include "rendering_attributtes.h"
#include "framebufferproperties.h"
#include <list>
#include <functional>

struct RenderStep
{
    std::list<int> objects;
    std::list<RenderTarget> targets;
    RenderingAttributes attributes = defaultAttributes;
    FrameBufferProperties frameBufferProperties;
    Program* prog;
    std::list<Texture> genericTextures;
    ConfigFunction preConfig = defaultConfig;
    ConfigFunction postConfig = defaultConfig;
};

#endif // RENDERSTEP_H
