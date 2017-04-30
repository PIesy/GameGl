#include "drawableworldobject.h"

Mat4 DrawableWorldObject::GetPositionMatrix()
{
    Mat4 result(1.0f);

    result[3] = Vec4(getPosition(), 1.0f);
    result[0][0] = scale;
    result[1][1] = scale;
    result[2][2] = scale;
    return result;
}

float DrawableWorldObject::GetScale() const
{
    return scale;
}

void DrawableWorldObject::SetScale(float value)
{
    scale = value;
}

Mat4 DrawableWorldObject::GetRotation() const
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

Mat4 DrawableWorldObject::GetWorldRotation() const
{
    return worldRotation;
}

void DrawableWorldObject::SetWorldRotation(const Mat4& value)
{
    worldRotation = value;
}

bool DrawableWorldObject::operator==(const DrawableWorldObject& rhs)
{
    return rhs.mesh.GetId() == mesh.GetId();
}

DrawableWorldObject::DrawableWorldObject()
{

}

Mesh& DrawableWorldObject::GetMesh()
{
    return mesh;
}

void DrawableWorldObject::setMesh(const Mesh& mesh)
{
    DrawableWorldObject::mesh = mesh;
}