#ifndef SDLINPUT_H
#define SDLINPUT_H

#include "../../../Input/inputapi.h"
#include "../../../Core/service/service.h"
#include "../../../Core/events/eventshandler.h"
#include "../../../Core/worker.h"
#include "../../../Core/enginecore.h"
#include <SDL2/SDL.h>

class SDLInputService: public Service
{
    Executor& serviceThread = core::core.Get().GetExecutor(true, getClassName<SDLInputService>());
    EventHandler& handler;
    bool stop = true;
    bool pause = false;
    std::mutex mutex;
    std::condition_variable suspension;
    void sdlInputRoutine();
    WindowData parseWindowEvent(SDL_Event& event);
    KeyboardData parseKeyboardEvent(SDL_Event& event);
    MouseData parseMouseEvent(SDL_Event& event);
public:
    explicit SDLInputService(EventHandler& handler);
    void Start() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    void Restart() override;
    void Wait() override;
};

class SDLInput: public InputApi
{
public:
    ~SDLInput() {}
    KeyboardData GetKeyboardState();
    MouseData GetMouseState();
};

#endif // SDLINPUT_H

