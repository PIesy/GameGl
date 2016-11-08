#include "drawableworldobject.h"

GraphicsObject& DrawableWorldObject::getObject()
{
    return object;
}

void DrawableWorldObject::setObject(const GraphicsObject& value)
{
    object = value;
}

Mat4 DrawableWorldObject::GetPositionMatrix()
{
    Mat4 result(1.0f);

    result[3] = Vec4(getPosition(), 1.0f);
    result[0][0] = scale;
    result[1][1] = scale;
    result[2][2] = scale;
    return result;
}

float DrawableWorldObject::getScale() const
{
    return scale;
}

void DrawableWorldObject::setScale(float value)
{
    scale = value;
}

DrawableWorldObject::DrawableWorldObject()
{

}
