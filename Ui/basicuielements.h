#ifndef BASICUIELEMENTS_H
#define BASICUIELEMENTS_H

#include "elementinterface.h"
#include <list>

class Item: public Element
{
protected:
    bool hover = false;
    Events lastEvent = Events::None;
    int size[2] = {0,0};
    WindowState* window = nullptr;
    float factor[2] = {0,0};
    int position[2] = {0,0};
    VertexObject base;
    VertexObject graphics;
    std::list<std::shared_ptr<Invokable>> actions;
    void setFactor();
    void setData();
    void rescale();
    void reposition();
    void invokeActions();
    void setColor(RenderObject& target, RGBA_Color color);
public:
    Item(){}
    Item(int width, int height);
    void Update();
    VertexObject* getGraphics();
    void setPosition(int x, int y);
    void setState(int x, int y, bool click);
    void setAction(Events event, Invokable& action);
    void Bind(WindowState* window);
};

#endif // BASICUIELEMENTS_H
