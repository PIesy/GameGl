#ifndef IOCLASSES
#define IOCLASSES

#include "Core/eventshandler_new.h"

struct KeyboardData
{
    bool isPressed = false;
    int windowId;
    int scancode = -1;
};

class KeyboardEvent: public Event<KeyboardData>
{
public:
    KeyboardEvent(KeyboardData data):Event(data) {}
};

struct MouseData
{
    enum class Type {Motion, Button, Wheel};
    Type eventType;
    int windowId;
    int coordinates[2];
    int relativeCoordinates[2];
    short button;
    bool state;
};

class MouseEvent: public Event<MouseData>
{
public:
    MouseEvent(MouseData data):Event(data) {}
};

struct WindowData
{
    enum class Type {Close, Hide, Show, Move, Resize};
    Type eventType;
    int windowId;
    int coordinates[2];
};

class WindowEvent: public Event<WindowData>
{
public:
    WindowEvent(WindowData data):Event(data) {}
};

#endif // IOCLASSES

