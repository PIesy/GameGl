#ifndef SDLCLASSES
#define SDLCLASSES

#include "../../../Graphics/graphicsclasses.h"
#include "SDL2/SDL.h"
#include "../../../Core/service/service.h"
#include <string>
#include "../../../Helpers/sharedstate.h"
#include "../../../Core/worker.h"
#include "graphics_context_data.h"
#include "../../../Core/enginecore.h"
#include "../../../Helpers/genericcondition.h"


class SdlWindow: public BasicWindow, public Service
{
    SDL_Window* window = nullptr;
    Executor& worker = core::core.Get().GetExecutor(true, getClassName<SdlWindow>());
    std::shared_ptr<std::atomic_bool> terminate{new std::atomic_bool{false}};
    GenericCondition<bool> opened{false};
    std::string name;
    int width;
    int height;
    bool hidden;
    std::once_flag openedFlag;
public:
    SdlWindow(const std::string& name, int width, int height, bool hidden);
    ~SdlWindow() override;
    void Close() override;
    WindowSize GetSize() override;
    operator SDL_Window*();

    void Start() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    void Restart() override;
    void Wait() override;
    void WaitForStart() override;
};

class SdlGLContext: public RenderingContext
{
    SharedState<bool> isValid = true;
    SdlWindow* window;
    SDL_GLContext context;
    Executor& executor = core::core.Get().GetExecutor(true, getClassName<SdlGLContext>());
public:
    SdlGLContext(const SDL_GLContext& context, SdlWindow& window, SharedGlContextData& data);
    ~SdlGLContext() override;
    void Destroy() override;
    void Execute(const Invokable& invokable) override;
    void SetWindow(Window& window) override;
    void MakeCurrent() override;
    Window GetWindow() override;
    SdlWindow& getSdlWindow();
};

#endif // SDLCLASSES

