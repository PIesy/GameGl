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

Mat4 DrawableWorldObject::getRotation() const
{
    return rotation;
}

void DrawableWorldObject::setRotation(const Mat4& value)
{
    rotation = value;
}

void DrawableWorldObject::Rotate(float degree, Vec3 axis)
{
    glm::fquat rot = glm::fquat(1, 0, 0, 0);
    rotateQuat(rot, degree, axis);
    rotation = glm::mat4_cast(rot) * rotation;
}

Mat4 DrawableWorldObject::getWorldRotation() const
{
    return worldRotation;
}

void DrawableWorldObject::setWorldRotation(const Mat4& value)
{
    worldRotation = value;
}

bool DrawableWorldObject::operator ==(const DrawableWorldObject& rhs)
{
    return rhs.object.getAttribute("name") == object.getAttribute("name");
}

DrawableWorldObject::DrawableWorldObject()
{

}
