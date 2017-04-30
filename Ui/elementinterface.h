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
    virtual Mesh& GetGraphics() = 0;
    virtual void SetState(int x, int y, bool click) = 0;
    virtual void SetAction(Events event, const GenericInvokable& action) = 0;
    virtual void SetPosition(int x, int y) = 0;
    virtual void Bind(WindowState* window) = 0;
};

#endif // ELEMENTINTERFACE_H
