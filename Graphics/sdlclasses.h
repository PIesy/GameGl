#ifndef SDLCLASSES
#define SDLCLASSES

#include "graphicsclasses.h"
#include "SDL2/SDL.h"
#include "Core/service/service.h"
#include <string>
#include "Helpers/sharedstate.h"

class SdlWindow: public BasicWindow
{
    SDL_Window* window;
public:
    void Open(std::string name, int width, int height, bool hidden = false);
    void Close();
    WindowSize getSize();
    operator SDL_Window*();
};

class SdlGLContext: public RenderingContext
{
    SharedState<bool> isValid = true;
    SdlWindow window;
    SDL_GLContext context;
    Worker worker;
public:
    SdlGLContext(const SDL_GLContext& context, const SdlWindow& window);
    ~SdlGLContext();
    void Destroy();
    void Execute(const Invokable& invokable);
    void SetWindow(const Window& window);
    void MakeCurrent();
    Window getWindow();
    SdlWindow& getSdlWindow();
};

#endif // SDLCLASSES

