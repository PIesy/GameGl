#ifndef INPUTDATA_H
#define INPUTDATA_H

struct MouseData
{
    enum class Type {Motion, Button, Wheel};
    enum class Button {Left, Middle, Right, X1, X2, None};
    Type eventType;
    unsigned int windowId;
    int coordinates[2];
    int relativeCoordinates[2];
    Button button;
    bool state = false;
};

struct WindowData
{
    enum class Type {Close, Hide, Show, Move, Resize};
    Type eventType;
    unsigned int windowId;
    int coordinates[2];
};

struct KeyboardData
{
    bool isPressed = false;
    unsigned int windowId;
    int scancode = -1;
};

#endif // INPUTDATA_H

