#include "uielements.h"

void ButtonOnHover(void*, Button* btn);
void ButtonOnExit(void*, Button* btn);

Button::Button(int width, int height, StorageApi& api, RGBA_Color color)
{
    Action<void*> onHover(ButtonOnHover, std::placeholders::_1, this);
    Action<void*> onExit(ButtonOnExit, std::placeholders::_1, this);
    size[0] = width;
    size[1] = height;
    initShape(api);
    this->color = color;
    setColor(graphics, color);
    SetAction(Events::onHover, onHover);
    SetAction(Events::onExit, onExit);
}

void Button::initShape(StorageApi& api)
{
    base = Shapes::Rectangle(api);
    graphics = Shapes::Rectangle(api);
}

void Button::SetColor(RGBA_Color color)
{
   this->color = color;
   setColor(graphics, color);
}

void Button::SetHoverColor(RGBA_Color color)
{
    hoverColor = color;
}

void Button::hover()
{
    setColor(graphics, hoverColor);
}

void Button::normal()
{
    setColor(graphics, color);
}

void ButtonOnHover(void *, Button *btn)
{
    btn->hover();
}

void ButtonOnExit(void *, Button *btn)
{
    btn->normal();
}
