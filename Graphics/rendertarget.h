#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "graphicsclasses.h"

enum class UsableRenderTargets {SCREEN, TEXTURE};
enum class TextureType {Color, Depth, Stencil};

struct TextureParameters
{
    unsigned long width;
    unsigned long height;
    TextureType type;
};

struct RenderTarget
{
    UsableRenderTargets target = UsableRenderTargets::SCREEN;
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
        target = UsableRenderTargets::TEXTURE;
        textureParameters = parameters;
    }
};

class ScreenRenderTarget : public RenderTarget
{
public:
    ScreenRenderTarget()
    {
        target = UsableRenderTargets::SCREEN;
    }
};

#endif // RENDERTARGET_H
