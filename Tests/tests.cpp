#include "tests.h"
#include "Core/eventshandler_new.h"
#include "Logger/logger.h"
#include <iostream>
#include <functional>

class TestEvent: public Event<int>
{
public:
    TestEvent(int val):Event(val) {}
};

void testFunction(TestEvent* event);

void eventsHandlerTest()
{
    EventHandler* handler = new EventHandler();
    EventListener listener;
    Action test;
    test.bind((ActionFun)testFunction);

    listener.listenFor<TestEvent>();
    listener.setHandler(test);
    handler->setListener(listener);
    handler->ThrowEvent(new TestEvent(25));
}

void testFunction(TestEvent* event)
{
    Logger::Log("Test event cought " + std::to_string(event->getPayload()));
}
