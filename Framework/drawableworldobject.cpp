#include "drawableworldobject.h"
#include "../Math/mathhelpers.h"

DrawableWorldObject::DrawableWorldObject()
{

}

Mat4 DrawableWorldObject::GetPositionMatrix()
{
    Mat4 result(1.0f);

    result[3] = Vec4(GetPosition(), 1.0f);
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

void DrawableWorldObject::SetRotation(const Mat4& value)
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
    if (GetPosition() != rhs.GetPosition())
        return false;
    for (Mesh& mesh : meshes)
    {
        bool present = false;
        for (const Mesh& rmesh : rhs.meshes)
        {
            if (rmesh.GetId() == mesh.GetId())
            {
                present = true;
                break;
            }
        }
        if (!present)
            return false;
    }

    return true;
}

std::vector<Mesh>& DrawableWorldObject::GetMeshes()
{
    return meshes;
}

void DrawableWorldObject::SetMeshes(const std::vector<Mesh>& meshes)
{
    DrawableWorldObject::meshes = meshes;
}

int DrawableWorldObject::GetTiling() const
{
    return tiling;
}

void DrawableWorldObject::SetTiling(int tiling)
{
    DrawableWorldObject::tiling = tiling;
}

std::vector<ObjectHint>& DrawableWorldObject::GetHints()
{
    return hints;
}

void DrawableWorldObject::SetHints(const std::vector<ObjectHint>& hints)
{
    DrawableWorldObject::hints = hints;
}

bool DrawableWorldObject::IsAoTiling() const
{
    return aoTiling;
}

void DrawableWorldObject::SetAoTiling(bool aoTiling)
{
    DrawableWorldObject::aoTiling = aoTiling;
}

