#include "tests.h"
#include "../Core/events/eventshandler.h"
#include "../Logger/logger.h"

class TestEvent: public Event<int>
{
public:
    TestEvent(int val, int hint = -1):Event(val, hint) {}
};

void eventsHandlerTest()
{
//    EventHandler* handler = new EventHandler();
//    EventListener listener;
//    int x = 13;
//    Action<TestEvent*> test{std::bind(testFunction, std::placeholders::_1)};
//
//    listener.listenFor<TestEvent>();
//    listener.setHandler(test);
//    handler->setListener(listener);
//    testCc(handler);
}