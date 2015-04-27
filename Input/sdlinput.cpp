#include "sdlinput.h"
#include "inputevents.h"
#include "Logger/logger.h"
#include <cstdint>

MouseData::Button parseMouseButton(std::uint8_t button);

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
    KeyboardData k;
    MouseData m;

    while(!stop)
    {
        waitIf(pause, mutex, suspension);
        if (SDL_WaitEvent(&event))
            switch(event.type)
            {
            case SDL_WINDOWEVENT:
                w = parseWindowEvent(event);
                handler.ThrowEvent(new WindowEvent(w, integral(w.eventType)));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                k = parseKeyboardEvent(event);
                handler.ThrowEvent(new KeyboardEvent(k, k.scancode));
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
                m = parseMouseEvent(event);
                handler.ThrowEvent(new MouseEvent(m, integral(m.eventType)));
                break;
            }
    }
    Logger::Log("Input service stopped");
}

WindowData SDLInputService::parseWindowEvent(SDL_Event& event)
{
    WindowData result;

    switch(event.window.event)
    {
    case SDL_WINDOWEVENT_CLOSE:
        result.eventType = result.Type::Close;
        break;
    case SDL_WINDOWEVENT_RESIZED:
        result.eventType = result.Type::Resize;
        result.coordinates[0] = event.window.data1;
        result.coordinates[1] = event.window.data2;
        break;
    default:
        result.eventType = result.Type::Hide;
        break;
    }
    return result;
}

KeyboardData SDLInputService::parseKeyboardEvent(SDL_Event& event)
{
    KeyboardData result;

    result.isPressed = event.key.state == SDL_PRESSED;
    result.scancode = event.key.keysym.scancode;
    result.windowId = event.key.windowID;
    return result;
}

MouseData SDLInputService::parseMouseEvent(SDL_Event& event)
{
    MouseData result;

    result.coordinates[0] = event.button.x;
    result.coordinates[1] = event.button.y;
    result.windowId = event.motion.windowID;
    switch(event.type)
    {
    case SDL_MOUSEMOTION:
        result.eventType = result.Type::Motion;
        result.relativeCoordinates[0] = event.motion.xrel;
        result.relativeCoordinates[1] = event.motion.yrel;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        result.eventType = result.Type::Button;
        result.button = parseMouseButton(event.button.button);
        result.state = event.button.state == SDL_PRESSED;
        break;
    case SDL_MOUSEWHEEL:
        result.eventType = result.Type::Wheel;
    }
    return result;
}

void SDLInputService::Start()
{
    std::lock_guard<std::mutex> lock(mutex);
    if(stop)
    {
        stop = false;
        serviceThread.setTask(Task(&SDLInputService::sdlInputRoutine, this));
    }
}

void SDLInputService::Stop()
{
    stop = true;
    suspension.notify_all();
    serviceThread.Terminate();
}

void SDLInputService::Pause()
{
    std::lock_guard<std::mutex> lock(mutex);
    pause = true;
}

void SDLInputService::Resume()
{
    std::lock_guard<std::mutex> lock(mutex);
    pause = false;
    suspension.notify_all();
}

void SDLInputService::Restart()
{
    std::lock_guard<std::mutex> lock(mutex);
    stop = true;
    Wait();
    serviceThread.setTask(Task(&SDLInputService::sdlInputRoutine, this));
}

void SDLInputService::Wait()
{
    serviceThread.Join();
}

MouseData::Button parseMouseButton(std::uint8_t button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        return MouseData::Button::Left;
    case SDL_BUTTON_MIDDLE:
        return MouseData::Button::Middle;
    case SDL_BUTTON_RIGHT:
        return MouseData::Button::Right;
    case SDL_BUTTON_X1:
        return MouseData::Button::X1;
    case SDL_BUTTON_X2:
        return MouseData::Button::X2;
    default:
        return MouseData::Button::None;
    }
}

Service* SDLInput::getService()
{
    service = new SDLInputService(engine->getEventHandler());
    return service;
}

MouseData SDLInput::GetMouseState()
{
    MouseData result;

    SDL_GetMouseState(&result.coordinates[0], &result.coordinates[1]);
    return result;
}

KeyboardData SDLInput::GetKeyboardState()
{
    KeyboardData result;
    return result;
}
