#include "eventshandler.h"
#include <iostream>

void inputHandler(EventStorage* eventStorage, Uint8* compareWith);

Action::Action(void (*action)(void *, void *), void *output)
{
    f = action;
    this->output = output;
}

void Action::Invoke(void *event)
{
    f(event, output);
}

int Action::getBindpoint()
{
    return bindpoint;
}

void Action::setBindpoint(int bindpoint)
{
    this->bindpoint = bindpoint;
}

EventsHandler::EventsHandler()
{
    workers[0].setWork((void (*)(void*))windowHandler, &eventStorage[0]);
    workers[1].setWork((void (*)(void*))keyboardHandler, &eventStorage[1]);
    workers[2].setWork((void (*)(void*))mouseHandler, &eventStorage[2]);
}

void EventsHandler::BindAction(Action action, int eventType, int category)
{
    action.bindpoint = eventType;
    eventStorage[category].actions.push_front(action);
}

void EventsHandler::Terminate()
{
    for (int i = 0; i < 3; i++)
    {
        eventStorage[i].terminate = true;
        eventStorage[i].event.newEvent.notify_all();
    }
    terminate = true;
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
        for(Action& action : eventStorage->actions)
            if((*compareWith == action.getBindpoint()) || (action.getBindpoint() == -1))
                action.Invoke(new SDL_Event(eventStorage->event.event));
        eventStorage->event.newEvent.wait(lock);
    }
}

void EventPoller(EventsHandler *handler)
{
    std::cout << "event poller started\n";
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
    std::cout << "event poller stopped\n";
}
