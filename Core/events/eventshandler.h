#ifndef EVENTHANDLER_NEW_H
#define EVENTHANDLER_NEW_H

#include "eventshandler_interfaces.h"
#include <typeindex>
#include <unordered_map>
#include "../../Helpers/helpers.h"
#include "../worker.h"

using EventInvokable = GenericInvokable;
using EventFilter = std::function<bool(EventInterface*)>;
static const EventFilter allowAll = [](EventInterface*) { return true; };

class Invalid {};

template<typename T>
class Event: public EventInterface
{
    T payload;
public:
    Event(T payload, int hint = -1) : payload(payload)
    {
        SetHint(hint);
    }
    virtual ~Event() {}
    const T& getPayload() const { return payload; }
};

class EventListener
{
    std::type_index type = typeid(Invalid);
    EventInvokable handler;
    EventFilter filter;
public:
    template<typename T>
    void listenFor() { type = std::type_index(typeid(T)); }
    void listenFor(std::type_index type);
    std::type_index getEventType();
    void setHandler(const EventInvokable& handler, const EventFilter& filter = allowAll);
    void Process(EventInterface* event);
};

class EventHandler
{
    Executor& executor;
    std::type_index invalidType = typeid(Invalid);
    int currentId = 0;
    std::unordered_map<std::type_index, std::unordered_map<int, EventListener>> listeners;
    std::unordered_map<int, std::type_index> listener_ids;
    bool checkListenerType(EventListener listener);
    int createListener(std::type_index type, const EventInvokable& action, const EventFilter& filter);
    void throwEvent(EventInterface* event, std::type_index type);
public:
    EventHandler();

    template<typename T>
    void ThrowEvent(T&& event)
    {
        static std::type_index type = typeid(T);
        EventInterface* copy = new T(event);
        throwEvent(copy, type);
    }

    void ThrowEvent(EventInterface* event);
    int setListener(EventListener listener);

    template<typename T>
    int setListener(const EventInvokable& action, const EventFilter& filter = allowAll)
    {
        static std::type_index type = typeid(T);
        return createListener(type, action, filter);
    }

    bool removeListener(int listenerId);
};


#endif // EVENTHANDLER_NEW_H
