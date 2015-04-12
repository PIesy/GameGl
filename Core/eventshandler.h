#ifndef EVENTSHANDLER_H
#define EVENTSHANDLER_H

#include "SDL2/SDL.h"
#include <condition_variable>
#include <mutex>
#include <list>
#include "worker.h"

struct Event
{
    SDL_Event event;
    std::condition_variable newEvent;
};

class ActionOld
{
    void (*function)(void* arg, void* output);
    void* output;
    int bindpoint;
public:
    ActionOld(void (*action)(void* event, void* output), void* output);
    void Invoke(void* event);
    int getBindpoint();
    void setBindpoint(int bindpoint);

    friend class EventsHandler;
};

struct EventStorage
{
    bool terminate = false;
    std::mutex mutex;
    Event event;
    std::list<ActionOld> actions;
};

class EventsHandler
{
    static const int handlersCount = 3;
    bool terminate = false;
    EventStorage eventStorage[handlersCount];
    Worker hostThread;
    Worker workers[handlersCount];
public:
    EventsHandler();
    void BindAction(ActionOld action, int eventType, int category);
    void UnbindAction(ActionOld action, int eventType, int category);
    void Start();
    void Terminate();
    void WaitEnd();

    friend void EventPoller(EventsHandler* handler);
};

#endif // EVENTSHANDLER_H
