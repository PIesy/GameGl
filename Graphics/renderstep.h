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
    std::vector<std::reference_wrapper<Shader>> shaders;
    std::list<Texture> genericTextures;
    ShaderConfiguration preConfig = defaultConfig;
    ShaderConfiguration postConfig = defaultConfig;
};

#endif // RENDERSTEP_H
