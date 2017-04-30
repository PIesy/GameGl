#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "texture.h"

struct RenderTarget
{
    enum RenderTargets {SCREEN, TEXTURE};
    RenderTargets target = SCREEN;
    Texture texture;

    RenderTarget() {}

    RenderTarget(const Texture& texture)
    {
        target = TEXTURE;
        this->texture = texture;
    }

    RenderTarget(Texture&& texture)
    {
        target = TEXTURE;
        this->texture = texture;
    }

    bool operator ==(const RenderTarget& rhs)
    {
        return target == rhs.target;
    }
};

#endif // RENDERTARGET_H
