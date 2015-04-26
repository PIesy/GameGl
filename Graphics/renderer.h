#ifndef RENDERER_H
#define RENDERER_H

#include "renderqueue.h"
#include "renderdefs.h"

struct RenderData
{
    GLuint VBO;
    GLuint IBO;
    GLuint VAO;
};

class Renderer
{
    bool terminate = false;
    RenderQueue* queue;
    Scene* currentScene = nullptr;
    unsigned int index_count = 0;
    RenderData data;
    GraphicsData* graphicsData = nullptr;
    void init();
    void render();
    void draw();
public:
    Renderer(RenderQueue* queue, GraphicsData* data);
    void Start();
};

void startRenderer(RenderInitializer* initializer);

#endif // RENDERER_H
