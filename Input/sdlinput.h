#ifndef SDLINPUT_H
#define SDLINPUT_H

#include "inputapi.h"

class SDLInputService: public Service
{
    EventHandler& handler;
    void sdlInputRoutine();
    bool stop = false;
    bool pause = false;
    std::mutex mutex;
    std::condition_variable suspension;
public:
    SDLInputService(EventHandler& handler);
    void Start();
    void Stop();
    void Pause();
    void Restart();
    void Wait();
};

class SDLInput: public InputApi
{
public:
    ~SDLInput() {}
    KeyboardData GetKeyboardState();
    MouseData GetMouseState();
    Service* getService();
};

#endif // SDLINPUT_H

