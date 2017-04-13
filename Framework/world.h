#ifndef WORLD_H
#define WORLD_H

#include "worldobject.h"
#include "camera.h"
#include "drawableworldobject.h"
#include <list>

class World
{
    std::list<Camera> cameras;
    std::list<DrawableWorldObject> drawableObjects;
    Vec3 worldSize = {1000.0f, 1000.0f, 1000.0f};
    float scaleFactor = 0.1;
public:
    World();

    std::list<Camera> getCameras() const;
    void setCameras(const std::list<Camera>& value);

    Camera& addCamera(const Vec3& position);
    void removeCamera(Camera& cam);

    DrawableWorldObject& addObject(const GraphicsObject& obj, const Vec3& position);
    void removeObject(DrawableWorldObject& obj);

    std::list<DrawableWorldObject>& getDrawableObjects();

    Vec3 getWorldSize() const;
    void setWorldSize(const Vec3& value);

    float getScaleFactor() const;
    void setScaleFactor(float value);
};

#endif // WORLD_H
