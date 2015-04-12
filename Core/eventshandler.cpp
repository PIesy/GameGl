#include "eventshandler.h"
#include "Logger/logger.h"

void inputHandler(EventStorage* eventStorage, Uint8* compareWith);
void windowHandler(EventStorage *storage);
void keyboardHandler(EventStorage* eventStorage);
void mouseHandler(EventStorage* eventStorage);
void EventPoller(EventsHandler* handler);

ActionOld::ActionOld(void (*fun)(void *, void *), void *output)
{
    function = fun;
    this->output = output;
}

void ActionOld::Invoke(void *event)
{
    function(event, output);
}

int ActionOld::getBindpoint()
{
    return bindpoint;
}

void ActionOld::setBindpoint(int bindpoint)
{
    this->bindpoint = bindpoint;
}

EventsHandler::EventsHandler()
{
    workers[0].setTask((ActionFun)windowHandler, &eventStorage[0]);
    workers[1].setTask((ActionFun)keyboardHandler, &eventStorage[1]);
    workers[2].setTask((ActionFun)mouseHandler, &eventStorage[2]);
}

void EventsHandler::BindAction(ActionOld action, int eventType, int category)
{
    action.bindpoint = eventType;
    eventStorage[category].actions.push_front(action);
}

void EventsHandler::Start()
{
    hostThread.setTask((ActionFun)EventPoller, this);
}

void EventsHandler::Terminate()
{
    for (int i = 0; i < handlersCount; i++)
    {
        eventStorage[i].terminate = true;
        eventStorage[i].event.newEvent.notify_all();
    }
    terminate = true;
    hostThread.Terminate();
}

void EventsHandler::WaitEnd()
{
    hostThread.Join();
}

void windowHandler(EventStorage* eventStorage)
{
    inputHandler(eventStorage, (Uint8*)&eventStorage->event.event.window.event);
}

void keyboardHandler(EventStorage* eventStorage)
{
    inputHandler(eventStorage, (Uint8*)&eventStorage->event.event.key.keysym.scancode);
}

void mouseHandler(EventStorage* eventStorage)
{
    inputHandler(eventStorage, (Uint8*)&eventStorage->event.event.button.button);
}

void inputHandler(EventStorage *eventStorage, Uint8* compareWith)
{
    std::unique_lock<std::mutex> lock(eventStorage->mutex);

    while(!eventStorage->terminate)
    {
        for(ActionOld& action : eventStorage->actions)
            if((*compareWith == action.getBindpoint()) || (action.getBindpoint() == -1))
                action.Invoke(new SDL_Event(eventStorage->event.event));
        eventStorage->event.newEvent.wait(lock);
    }
}

void EventPoller(EventsHandler *handler)
{
    Logger::Log("event poller started");
    SDL_Event event;
    int newEvent = 0;

    while(!handler->terminate)
    {
        newEvent = SDL_PollEvent(&event);
        if (newEvent)
        {
            switch(event.type) {
            case SDL_WINDOWEVENT:
                handler->eventStorage[0].event.event = event;
                handler->eventStorage[0].event.newEvent.notify_all();
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handler->eventStorage[1].event.event = event;
                handler->eventStorage[1].event.newEvent.notify_all();
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
                handler->eventStorage[2].event.event = event;
                handler->eventStorage[2].event.newEvent.notify_all();
                break;
            }
        }
        SDL_Delay(1);
    }
    Logger::Log("event poller stopped");
}
