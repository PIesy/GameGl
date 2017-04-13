#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "graphicsclasses.h"

enum class RenderTargets {SCREEN, TEXTURE};
enum class TextureType {Color, Depth, Stencil};

struct TextureParameters
{
    unsigned width;
    unsigned height;
    TextureType type;
};

struct RenderTarget
{
    RenderTargets target = RenderTargets::SCREEN;
    TextureParameters textureParameters;

    bool operator ==(const RenderTarget& rhs)
    {
        return target == rhs.target;
    }
};

class TextureRenderTarget : public RenderTarget
{
public:
    TextureRenderTarget(const TextureParameters& parameters)
    {
        target = RenderTargets::TEXTURE;
        textureParameters = parameters;
    }
};

#endif // RENDERTARGET_H
