#ifndef RENDERER_H
#define RENDERER_H

#include "renderdefs.h"
#include "graphicsclasses.h"
#include "sdlclasses.h"
#include "Helpers/genericcondition.h"
#include <atomic>

using ViewportSize = Size;

struct RenderData
{
    GLuint VBO;
    GLuint IBO;
    GLuint VAO;
};

class GlRenderer: public Renderer
{
    SdlGLContext& context;
    bool terminate = true;
    bool empty = true;
    Scene currentScene;
    unsigned int index_count = 0;
    RenderData data;
    std::queue<Scene> renderQueue;
    std::atomic<ViewportSize> viewportSize;
    Task renderTask;
    GenericCondition<bool> pause = false;
    void renderLoop();
    void init();
    void render();
    void draw();
    void update();
public:
    GlRenderer(SdlGLContext& context);
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
    void SetWindow(const Window &window);
    void Draw(const Scene& scene);
    void SetViewport(int width, int height);
};

#endif // RENDERER_H
