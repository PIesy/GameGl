#ifndef DRAWABLEWORLDOBJECT_H
#define DRAWABLEWORLDOBJECT_H

#include "worldobject.h"

class DrawableWorldObject : public WorldObject
{
    GraphicsObject object;
    float scale = 1.0f;
public:
    DrawableWorldObject();
    GraphicsObject& getObject();
    void setObject(const GraphicsObject& value);
    Mat4 GetPositionMatrix();
    float getScale() const;
    void setScale(float value);
};

#endif // DRAWABLEWORLDOBJECT_H
