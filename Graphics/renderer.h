#ifndef RENDERER_H
#define RENDERER_H

#include "graphicsclasses.h"
#include "scene.h"

class Renderer: public Service
{
public:
    virtual void SetWindow(Window& window) = 0;
    virtual void Draw(const Scene& path) = 0;
    virtual void SetViewport(int width, int height) = 0;
};

#endif // RENDERER_H
