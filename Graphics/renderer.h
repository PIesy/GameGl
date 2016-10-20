#ifndef RENDERER_H
#define RENDERER_H

#include "renderdefs.h"
#include "graphicsclasses.h"
#include "sdlclasses.h"
#include "Helpers/genericcondition.h"
#include "glhelpers.h"
#include <atomic>
#include <mutex>
#include <list>
#include <unordered_map>
#include <string>

using ViewportSize = Size;

struct RendererData
{
    std::unordered_map<std::string, glhelpers::VertexArrayObject> VAOs;
    GLuint VBO;
    GLuint IBO;
    GLuint VAO;
};

class GlRenderer: public Renderer
{
    std::mutex queueLock;
    SdlGLContext& context;
    bool terminate = true;
    bool empty = true;
    Scene currentScene;
    unsigned int index_count = 0;
    RendererData data;
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
