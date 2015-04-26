#ifndef EVENTHANDLER_NEW_H
#define EVENTHANDLER_NEW_H

#include "eventshandler_interfaces.h"
#include <typeindex>
#include <unordered_map>
#include "Helpers/helpers.h"

using EventInvokable = GenericInvokable;

class Invalid {};

template<typename T>
class Event: public EventInterface
{
    T payload;
public:
    Event(T payload) { this->payload = payload; }
    virtual ~Event() {}
    const T& getPayload() const { return payload; }
};

class EventListener
{
    std::type_index type = typeid(Invalid);
    EventInvokable handler;
public:
    template<typename T>
    void listenFor() { type = std::type_index(typeid(T)); }
    void listenFor(std::type_index type);
    std::type_index getEventType();
    void setHandler(const EventInvokable& handler);
    void Process(EventInterface* event);
};

class EventHandler
{
    std::type_index invalidType = typeid(Invalid);
    int currentId = 0;
    std::unordered_map<std::type_index, std::unordered_map<int, EventListener>> listeners;
    std::unordered_map<int, std::type_index> listener_ids;
    bool checkListenerType(EventListener listener);
    int createListener(std::type_index type, const EventInvokable& action);
public:
    void ThrowEvent(EventInterface* event);
    int setListener(EventListener listener);
    template<typename T>
    int setListener(const EventInvokable& action) { return createListener(getType<T>(), action); }
    bool removeListener(int listenerId);
};


#endif // EVENTHANDLER_NEW_H
