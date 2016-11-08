#ifndef RENDERER_H
#define RENDERER_H

#include "graphicsclasses.h"
#include "renderpath.h"

class Renderer: public Service
{
public:
    virtual void SetWindow(const Window& window) = 0;
    virtual void Draw(const RenderPath& path) = 0;
    virtual void SetViewport(int width, int height) = 0;
};

#endif // RENDERER_H
