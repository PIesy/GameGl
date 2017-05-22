#ifndef WORLD_H
#define WORLD_H

#include "worldobject.h"
#include "camera.h"
#include "drawableworldobject.h"
#include "light.h"
#include <list>

class World
{
    std::list<Camera> cameras;
    std::list<DrawableWorldObject> drawableObjects;
    std::list<Light> lights;
    Vec3 worldSize = {1000.0f, 1000.0f, 1000.0f};
    float scaleFactor = 1;
public:
    World();

    std::list<Camera> GetCameras() const;
    void SetCameras(const std::list<Camera>& value);
    Camera& AddCamera(const Vec3& position);
    void RemoveCamera(Camera& cam);

    DrawableWorldObject& AddObject(const Mesh& obj, const Vec3& position);
    DrawableWorldObject& AddObject(const std::vector<Mesh>& obj, const Vec3& position);
    void removeObject(DrawableWorldObject& obj);
    std::list<DrawableWorldObject>& GetDrawableObjects();

    Light& AddLight(const Vec3& position, const Vec3& color);
    std::list<Light>& GetLights();

    Vec3 GetWorldSize() const;
    void SetWorldSize(const Vec3& value);
    float GetScaleFactor() const;
    void SetScaleFactor(float value);
};

#endif // WORLD_H
