#ifndef IOCLASSES
#define IOCLASSES

#include "Core/eventshandler_new.h"
#include "inputdata.h"

class KeyboardEvent: public Event<KeyboardData>
{
public:
    KeyboardEvent(KeyboardData data):Event(data) {}
};

class MouseEvent: public Event<MouseData>
{
public:
    MouseEvent(MouseData data):Event(data) {}
};

class WindowEvent: public Event<WindowData>
{
public:
    WindowEvent(WindowData data):Event(data) {}
};

#endif // IOCLASSES

