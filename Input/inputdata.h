#ifndef INPUTDATA_H
#define INPUTDATA_H

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

struct WindowData
{
    enum class Type {Close, Hide, Show, Move, Resize};
    Type eventType;
    int windowId;
    int coordinates[2];
};

struct KeyboardData
{
    bool isPressed = false;
    int windowId;
    int scancode = -1;
};

#endif // INPUTDATA_H

