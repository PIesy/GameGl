#ifndef GL_RENDERER_H
#define GL_RENDERER_H

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
    bool firstUse = true;
    Scene currentScene;
    std::queue<Scene> renderQueue;
    std::atomic<ViewportSize> viewportSize;
    Task renderTask;
    GenericCondition<bool> pause{false};
    GlContext glContext;

    void renderLoop();
    void init();
    void render();
    void draw();
    void update();
public:
    explicit GlRenderer(SdlGLContext& context);
    void Start() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    void Restart() override;
    void Wait() override;
    void SetWindow(Window& window) override;
    void Draw(const Scene& path) override;
    void SetViewport(int width, int height) override;
};

#endif // GL_RENDERER_H
