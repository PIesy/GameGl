#ifndef SDLINPUT_H
#define SDLINPUT_H

#include "inputapi.h"
#include <SDL2/SDL.h>

class SDLInputService: public Service
{
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
    SDLInputService(EventHandler& handler);
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Restart();
    void Wait();
};

class SDLInput: public InputApi
{
    Service* service;
public:
    ~SDLInput() {}
    KeyboardData GetKeyboardState();
    MouseData GetMouseState();
    Service* getService();
};

#endif // SDLINPUT_H

