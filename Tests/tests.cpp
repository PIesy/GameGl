#include "tests.h"
#include "Core/eventshandler.h"
#include "Logger/logger.h"
#include <iostream>
#include <functional>

class TestEvent: public Event<int>
{
public:
    TestEvent(int val, int hint = -1):Event(val, hint) {}
};

void testCc(EventHandler* handler);

void testFunction(TestEvent* event, int* val);

void eventsHandlerTest()
{
    EventHandler* handler = new EventHandler();
    EventListener listener;
    Action<TestEvent*> test;
    int x = 13;
    test.Bind(testFunction, std::placeholders::_1, &x);

    listener.listenFor<TestEvent>();
    listener.setHandler(test);
    handler->setListener(listener);
    testCc(handler);
}

void testCc(EventHandler *handler)
{
    handler->ThrowEvent(new TestEvent(25));
}

void testFunction(TestEvent* event, int* val)
{
    Logger::Log("Test event cought " + std::to_string(event->getPayload()) + std::to_string(*val));
}
