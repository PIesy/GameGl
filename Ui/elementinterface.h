#ifndef ELEMENTINTERFACE_H
#define ELEMENTINTERFACE_H

#include "../Graphics/graphicsapi.h"
#include "basicshapes.h"
#include "../Core/engineinterface.h"
#include "../Helpers/helpers.h"

enum class Events:int {None, onHover, onClick, onExit, onEnter};

struct WindowState
{
    int resolution[2] = {600,600};
};

class Element
{
public:
    virtual ~Element(){}
    virtual void Update() = 0;
    virtual VertexObject* getGraphics() = 0;
    virtual void setState(int x, int y, bool click) = 0;
    virtual void setAction(Events event, ActionOld action) = 0;
    virtual void setPosition(int x, int y) = 0;
    virtual void Bind(WindowState* window) = 0;
};

#endif // ELEMENTINTERFACE_H
