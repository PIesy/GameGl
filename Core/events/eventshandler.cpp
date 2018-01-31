#include "eventshandler.h"
#include "../../Logger/logger.h"
#include "../enginecore.h"

const auto logger = Logger::GetLogger(getClassName<EventHandler>());

void EventListener::listenFor(std::type_index type)
{
    this->type = type;
}

std::type_index EventListener::getEventType()
{
    return type;
}

void EventListener::Process(EventInterface* event)
{
    if (filter(event))
        handler.InvokeWithArgs(event);
}

void EventListener::setHandler(const EventInvokable& handler, const EventFilter& filter)
{
    this->handler = handler;
    this->filter = filter;
}

int EventHandler::setListener(EventListener listener)
{
    std::type_index type = listener.getEventType();

    if (checkListenerType(listener))
        return -1;
    listener_ids.insert({currentId, type});
    if (!listeners.count(type))
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
    if (listener_ids.count(listenerId))
    {
        std::type_index type = listener_ids.at(listenerId);
        listeners.at(type).erase(listenerId);
        listener_ids.erase(listenerId);
        return true;
    }
    return false;
}

int EventHandler::createListener(std::type_index type, const EventInvokable& action, const EventFilter& filter)
{
    EventListener listener;

    listener.listenFor(type);
    listener.setHandler(action, filter);
    return setListener(listener);
}

void EventHandler::throwEvent(EventInterface* event, std::type_index type, std::once_flag& flag)
{
    if (listeners.find(type) != listeners.end())
    {
        executor.Execute(Task([event, this, type, &flag]()
        {
            std::unique_ptr<EventInterface> packedEvent(event);
            for (std::pair<const int, EventListener>& listener: listeners.at(type))
                listener.second.Process(packedEvent.get());
        }));
    }
    else
    {
        std::call_once(flag, [event]
        {
            logger.LogDebug(std::string("No listeners registered for ") + getType(*event).name());
        });
    }
}

EventHandler::EventHandler() : executor(core::core.Get().GetExecutor())
{

}
