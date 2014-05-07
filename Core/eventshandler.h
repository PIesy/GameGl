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

class Action
{
    void (*f)(void* arg, void* output);
    void* output;
    int bindpoint;
public:
    Action(void (*action)(void* event, void* output), void* output);
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
    std::list<Action> actions;
};

class EventsHandler
{
    bool terminate = false;
    EventStorage eventStorage[3];
    Worker workers[3];
public:
    EventsHandler();
    void BindAction(Action action, int eventType, int category);
    void Terminate();

    friend void EventPoller(EventsHandler* handler);
};

void windowHandler(EventStorage *storage);
void keyboardHandler(EventStorage* eventStorage);
void mouseHandler(EventStorage* eventStorage);

void EventPoller(EventsHandler* handler);

#endif // EVENTSHANDLER_H
