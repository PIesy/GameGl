#ifndef SDLCLASSES
#define SDLCLASSES

#include "../../../Graphics/graphicsclasses.h"
#include "SDL2/SDL.h"
#include "../../../Core/service/service.h"
#include <string>
#include "../../../Helpers/sharedstate.h"
#include "../../../Core/worker.h"
#include "graphicscontextdata.h"
#include "../../../Core/enginecore.h"


class SdlWindow: public BasicWindow, public Service
{
    SDL_Window* window;
    Executor* worker;
public:
    SdlWindow();
    void Open(std::string name, int width, int height, bool hidden = false);
    void Close();
    WindowSize getSize();
    operator SDL_Window*();

    void Start() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    void Restart() override;
    void Wait() override;
};

class SdlGLContext: public RenderingContext
{
    SharedState<bool> isValid = true;
    SdlWindow window;
    SDL_GLContext context;
    Executor& executor = core::core.Get().GetExecutor(true, "RenderingContext");
public:
    SdlGLContext(const SDL_GLContext& context, const SdlWindow& window, GraphicsContextData& data);
    ~SdlGLContext();
    void Destroy();
    void Execute(const Invokable& invokable);
    void SetWindow(const Window& window);
    void MakeCurrent();
    Window getWindow();
    SdlWindow& getSdlWindow();
};

#endif // SDLCLASSES

