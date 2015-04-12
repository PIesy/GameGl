#include "eventshandler_new.h"
#include "Helpers/helpers.h"

using namespace std;

type_index EventListener::getEventType()
{
    return type;
}

void EventListener::Process(EventInterface* event)
{
    handler.invoke(event);
}

void EventListener::setHandler(Action handler)
{
    this->handler = handler;
}

int EventHandler::setListener(EventListener listener)
{
    type_index type = listener.getEventType();

    if(checkListenerType(listener))
        return -1;
    listener_ids.insert({currentId, type});
    if(!listeners.count(type))
    {
        std::unordered_map<int, EventListener> inner;
        listeners.insert({type, inner});
    }
    listeners.at(type).insert({currentId, listener});
    return currentId++;
}

bool EventHandler::checkListenerType(EventListener listener)
{
    return invalidType == listener.getEventType();
}

bool EventHandler::removeListener(int listenerId)
{
    if(listener_ids.count(listenerId))
    {
        type_index type = listener_ids.at(listenerId);
        listeners.at(type).erase(listenerId);
        listener_ids.erase(listenerId);
        return true;
    }
    return false;
}

void EventHandler::ThrowEvent(EventInterface *event)
{
    try
    {
        for(std::pair<const int, EventListener>& listener: listeners.at(getType(*event)))
            listener.second.Process(event);
    }
    catch(out_of_range) {}
}
