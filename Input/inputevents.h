#ifndef IOCLASSES
#define IOCLASSES

#include "Core/eventshandler.h"
#include "inputdata.h"

class KeyboardEvent: public Event<KeyboardData>
{
public:
    KeyboardEvent(KeyboardData data, int hint = -1):Event(data, hint) {}
};

class MouseEvent: public Event<MouseData>
{
public:
    MouseEvent(MouseData data, int hint = -1):Event(data, hint) {}
};

class WindowEvent: public Event<WindowData>
{
public:
    WindowEvent(WindowData data, int hint = -1):Event(data, hint) {}
};

#endif // IOCLASSES

