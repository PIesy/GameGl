#include "basicuielements.h"
#include <iostream>

Item::Item(int width, int height)
{
    size[0] = width;
    size[1] = height;
    setData();
}

void Item::setData()
{
    base = Shapes::Box();
    graphics = Shapes::Box();
}

void Item::setFactor()
{
    factor[0] = size[0] * 1.0 / window->resolution[0];
    factor[1] = size[1] * 1.0 / window->resolution[1];
}

void Item::rescale()
{
    unsigned int i;

    for(i = 0; i < graphics.data().vertices.size(); i++)
    {
        graphics.data().vertices[i].coords[0] = (base.data().vertices[i].coords[0] + 1) * factor[0] - 1;
        graphics.data().vertices[i].coords[1] = (base.data().vertices[i].coords[1] + 1) * factor[1] - 1;
    }
}

void Item::reposition()
{
    unsigned int i;

    for(i = 0; i < graphics.data().vertices.size(); i++)
    {
        graphics.data().vertices[i].coords[0] += position[0] * 2.0 / window->resolution[0];
        graphics.data().vertices[i].coords[1] += position[1] * 2.0 / window->resolution[1];
    }
}

void Item::setColor(RenderData& target, RGBA_Color color)
{
    unsigned int i, j;

    for (i = 0; i < target.vertices.size(); i++)
        for (j = 0; j < 4; j ++)
            target.vertices[i].color[j] = color.color[j];
}

void Item::Update()
{
    setFactor();
    rescale();
    reposition();
}

GraphicsObject* Item::getGraphics()
{
    return &graphics;
}

void Item::setPosition(int x, int y)
{
    position[0] = x;
    position[1] = y;
    reposition();
}

void Item::setState(int x, int y, bool click)
{
    y = window->resolution[1] - y;
    if ((x > position[0]) && (x < position[0] + size[0]) && (y > position[1]) && (y < position[1] + size[1]))
    {
        if (!hover)
            lastEvent = Events::onEnter;
        else
            lastEvent = Events::onHover;
        hover = true;
        if (click)
            lastEvent = Events::onClick;
        invokeActions();
    }
    else
    {
        if (hover)
            lastEvent = Events::onExit;
        else
            lastEvent = Events::None;
        hover = false;
        invokeActions();
    }
}

void Item::invokeActions()
{
    for (auto& action : actions)
    {
        if (action.first == integral(lastEvent))
            action.second.InvokeWithArgs(this);
    }
}

void Item::setAction(Events event, const GenericInvokable& action)
{
    actions.emplace(integral(event), action);
}

void Item::Bind(WindowState* window)
{
    this->window = window;
    setFactor();
    rescale();
}
