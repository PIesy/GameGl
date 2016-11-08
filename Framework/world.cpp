#include "world.h"
#include "Graphics/graphicsclasses.h"
#include <functional>

bool operator ==(const std::reference_wrapper<Camera>& lhs, const std::reference_wrapper<Camera>& rhs)
{
    return lhs == rhs;
}

bool operator ==(const std::reference_wrapper<DrawableWorldObject>& lhs, const std::reference_wrapper<DrawableWorldObject>& rhs)
{
    return lhs == rhs;
}

std::list<Camera> World::getCameras() const
{
    return cameras;
}

void World::setCameras(const std::list<Camera>& value)
{
    cameras = value;
}

Camera& World::addCamera(const Vec3& position)
{
    Camera cam;

    cam.setPosition(position);
    cameras.push_back(cam);
    return cameras.back();
}

void World::removeCamera(Camera& cam)
{
    cameras.remove(cam);
}

DrawableWorldObject& World::addObject(const GraphicsObject& obj, const Vec3& position)
{
    DrawableWorldObject drawableObject;

    drawableObject.setObject(obj);
    drawableObject.setPosition(position);
    drawableObject.setScale(scaleFactor);
    drawableObjects.push_back(drawableObject);
    return drawableObjects.back();
}

void World::removeObject(DrawableWorldObject& obj)
{
    drawableObjects.remove(obj);
}

std::list<DrawableWorldObject>& World::getDrawableObjects()
{
    return drawableObjects;
}

Vec3 World::getWorldSize() const
{
    return worldSize;
}

void World::setWorldSize(const Vec3& value)
{
    worldSize = value;
}

float World::getScaleFactor() const
{
    return scaleFactor;
}

void World::setScaleFactor(float value)
{
    scaleFactor = value;
}

World::World()
{
    
}
