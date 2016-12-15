#ifndef DRAWABLEWORLDOBJECT_H
#define DRAWABLEWORLDOBJECT_H

#include "worldobject.h"

class DrawableWorldObject : public WorldObject
{
    GraphicsObject object;
    float scale = 1.0f;
    Mat4 rotation;
    Mat4 worldRotation;
public:
    DrawableWorldObject();
    GraphicsObject& getObject();
    void setObject(const GraphicsObject& value);
    Mat4 GetPositionMatrix();
    float getScale() const;
    void setScale(float value);
    Mat4 getRotation() const;
    void setRotation(const Mat4& value);
    void Rotate(float degree, Vec3 axis);
    Mat4 getWorldRotation() const;
    void setWorldRotation(const Mat4& value);
    bool operator ==(const DrawableWorldObject& rhs);
};

#endif // DRAWABLEWORLDOBJECT_H
