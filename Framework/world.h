#ifndef WORLD_H
#define WORLD_H

#include "worldobject.h"
#include "camera.h"
#include "drawableworldobject.h"
#include "light.h"
#include "material.h"
#include <list>

class World
{
    std::list<Camera> cameras;
    std::list<DrawableWorldObject> drawableObjects;
    std::vector<Light> pointLights;
    std::vector<Light> directionalLights;
    std::vector<Light> spotlights;
    Vec3 worldSize = {1000.0f, 1000.0f, 1000.0f};
    float scaleFactor = 1;
public:
    World();

    std::list<Camera> GetCameras() const;
    void SetCameras(const std::list<Camera>& value);
    Camera& AddCamera(const Vec3& position);
    void RemoveCamera(Camera& cam);

    DrawableWorldObject& AddObject(const Mesh& obj, const Vec3& position, const Material& overrideMaterial = {});
    DrawableWorldObject& AddObject(const std::vector<Mesh>& obj, const Vec3& position, const Material& overrideMaterial = {});
    void removeObject(DrawableWorldObject& obj);
    std::list<DrawableWorldObject>& GetDrawableObjects();

    Light& AddPointLight(const Vec3& position, const Vec3& color);
    Light& AddSpotlight(const Vec3& position, const Vec3& color, const Vec3& direction, float angle);
    Light& AddDirectionalLight(const Vec3& color, const Vec3& direction);
    std::vector<Light>& GetPointLights();
    std::vector<Light>& GetDirectionalLights();
    std::vector<Light>& GetSpotights();

    Vec3 GetWorldSize() const;
    void SetWorldSize(const Vec3& value);
    float GetScaleFactor() const;
    void SetScaleFactor(float value);
};

#endif // WORLD_H
