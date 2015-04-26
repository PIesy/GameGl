#include "sdlinput.h"
#include "SDL2/SDL.h"
#include "inputevents.h"
#include "Logger/logger.h"


SDLInputService::SDLInputService(EventHandler& handler):handler(handler)
{
    if(!SDL_WasInit(SDL_INIT_EVENTS))
        SDL_Init(SDL_INIT_EVENTS);
}

void SDLInputService::sdlInputRoutine()
{
    Logger::Log("Input service started");
    SDL_Event event;
    WindowData w;
    MouseData m;
    KeyboardData k;

    while(!stop)
    {
        waitIf(pause, mutex, suspension);
        if (SDL_WaitEvent(&event))
            switch(event.type)
            {
            case SDL_WINDOWEVENT:
                w.eventType = event.window.event == SDL_WINDOWEVENT_CLOSE ? w.Type::Close : w.Type::Resize;
                handler.ThrowEvent(new WindowEvent(w));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                k.isPressed = event.key.state == SDL_PRESSED;
                k.scancode = event.key.keysym.scancode;
                k.windowId = event.key.windowID;
                handler.ThrowEvent(new KeyboardEvent(k));
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
                m.eventType = m.Type::Motion;
                handler.ThrowEvent(new MouseEvent(m));
                break;
            }
    }
    Logger::Log("Input service stopped");
}

void SDLInputService::Start()
{
    serviceThread.setTask(Task(&SDLInputService::sdlInputRoutine, this));
}

void SDLInputService::Stop()
{
    stop = true;
    suspension.notify_all();
}

void SDLInputService::Pause()
{
    pause = true;
    suspension.notify_all();
}

void SDLInputService::Restart()
{

}

void SDLInputService::Wait()
{
    serviceThread.Join();
}
