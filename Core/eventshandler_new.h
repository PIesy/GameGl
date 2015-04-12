#ifndef EVENTHANDLER_NEW_H
#define EVENTHANDLER_NEW_H

#include "invokable.h"
#include "eventshandler_interfaces.h"
#include <typeindex>
#include <unordered_map>

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
    Action handler;
public:
    template<typename T>
    void listenFor() { type = std::type_index(typeid(T)); }
    std::type_index getEventType();
    void setHandler(Action handler);
    void Process(EventInterface* event);
};

class EventHandler
{
    std::type_index invalidType = typeid(Invalid);
    int currentId = 0;
    std::unordered_map<std::type_index, std::unordered_map<int, EventListener>> listeners;
    std::unordered_map<int, std::type_index> listener_ids;
    bool checkListenerType(EventListener listener);
public:
    void ThrowEvent(EventInterface* event);
    int setListener(EventListener listener);
    bool removeListener(int listenerId);
};


#endif // EVENTHANDLER_NEW_H
