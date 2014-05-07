#include "uielements.h"

void ButtonOnHover(void*, Button* btn);
void ButtonOnExit(void*, Button* btn);

Button::Button(int width, int height, RGBA_Color color)
{
    Action onHover((void (*)(void*, void*))ButtonOnHover, this);
    Action onExit((void (*)(void*, void*))ButtonOnExit, this);
    size[0] = width;
    size[1] = height;
    initShape();
    this->color = color;
    setColor(graphics, color);
    setAction(Events::onHover, onHover);
    setAction(Events::onExit, onExit);
}

void Button::initShape()
{
    base = Shapes::Box();
    graphics = Shapes::Box();
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
