#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "texture.h"

struct RenderTarget
{
    enum RenderTargets {SCREEN, TEXTURE};
    RenderTargets target = SCREEN;
    Texture texture;

    RenderTarget() {}

    explicit RenderTarget(const Texture& texture) : texture(texture)
    {
        target = TEXTURE;
    }

    explicit RenderTarget(Texture&& texture) : texture(texture)
    {
        target = TEXTURE;
    }

    bool operator ==(const RenderTarget& rhs)
    {
        return target == rhs.target;
    }
};

#endif // RENDERTARGET_H
