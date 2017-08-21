#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "../../../Graphics/renderdefs.h"
#include "../../../Graphics/graphicsclasses.h"
#include "sdlclasses.h"
#include "../../../Graphics/renderer.h"
#include "../../../Helpers/task.h"
#include "../../../Helpers/genericcondition.h"
#include "glcontext.h"
#include <atomic>
#include <mutex>
#include <list>
#include <unordered_map>
#include <string>

class GlRenderer: public Renderer
{
    using ViewportSize = Size;

    std::mutex queueLock;
    SdlGLContext& context;
    bool terminated = true;
    bool emptyPath = true;
    Scene currentScene;
    std::queue<Scene> renderQueue;
    std::atomic<ViewportSize> viewportSize;
    Task renderTask;
    GenericCondition<bool> pause = false;
    GlContext glContext;

    void renderLoop();
    void init();
    void render();
    void draw();
    void update();
public:
    explicit GlRenderer(SdlGLContext& context);
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
    void SetWindow(const Window &window);
    void Draw(const Scene& path);
    void SetViewport(int width, int height);
};

#endif // GL_RENDERER_H
