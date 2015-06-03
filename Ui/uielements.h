#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "basicuielements.h"

class Button: public Item
{
    RGBA_Color color;
    RGBA_Color hoverColor = Vec4{1,0,0,1};
    void initShape();
    void hover();
    void normal();
public:
    Button(int width, int height, RGBA_Color color = Vec4{1,1,1,1});
    void SetColor(RGBA_Color color);
    void SetHoverColor(RGBA_Color color);

    friend void ButtonOnHover(void*, Button* btn);
    friend void ButtonOnExit(void*, Button* btn);
};

#endif // UIELEMENTS_H
