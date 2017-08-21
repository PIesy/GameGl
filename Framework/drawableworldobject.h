#ifndef DRAWABLEWORLDOBJECT_H
#define DRAWABLEWORLDOBJECT_H

#include "worldobject.h"
#include "../Graphics/mesh.h"
#include "object_hints.h"

class DrawableWorldObject : public WorldObject
{
    std::vector<Mesh> meshes;
    float scale = 1.0f;
    Mat4 rotation{1};
    Mat4 worldRotation{1};
    int tiling = 1;
    bool aoTiling = true;
    std::vector<ObjectHint> hints;
public:
    DrawableWorldObject();

    std::vector<Mesh>& GetMeshes();
    void SetMeshes(const std::vector<Mesh>& meshes);
    Mat4 GetPositionMatrix();
    float GetScale() const;
    void SetScale(float value);
    Mat4 GetRotation() const;
    void SetRotation(const Mat4& value);
    void Rotate(float degree, Vec3 axis);
    Mat4 GetWorldRotation() const;
    void SetWorldRotation(const Mat4& value);
    bool operator==(const DrawableWorldObject& rhs);
    int GetTiling() const;
    void SetTiling(int tiling);
    std::vector<ObjectHint>& GetHints();
    void SetHints(const std::vector<ObjectHint>& hints);
    bool IsAoTiling() const;
    void SetAoTiling(bool aoTiling);
};

#endif // DRAWABLEWORLDOBJECT_H
