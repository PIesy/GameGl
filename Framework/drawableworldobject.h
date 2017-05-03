#ifndef DRAWABLEWORLDOBJECT_H
#define DRAWABLEWORLDOBJECT_H

#include "worldobject.h"
#include "../Graphics/mesh.h"
#include "renderflags.h"

class DrawableWorldObject : public WorldObject
{
    Mesh mesh;
    float scale = 1.0f;
    Mat4 rotation{1};
    Mat4 worldRotation{1};
    RenderFlags renderFlags = RenderFlags::Default;
public:
    DrawableWorldObject();

    Mesh& GetMesh();
    void SetMesh(const Mesh& mesh);
    Mat4 GetPositionMatrix();
    float GetScale() const;
    void SetScale(float value);
    Mat4 GetRotation() const;
    void SetRotation(const Mat4& value);
    void Rotate(float degree, Vec3 axis);
    Mat4 GetWorldRotation() const;
    void SetWorldRotation(const Mat4& value);
    bool operator==(const DrawableWorldObject& rhs);
    RenderFlags GetRenderFlags() const;
    void SetRenderFlags(RenderFlags renderFlags);
};

#endif // DRAWABLEWORLDOBJECT_H
