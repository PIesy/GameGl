#include "world.h"

bool operator ==(const std::reference_wrapper<Camera>& lhs, const std::reference_wrapper<Camera>& rhs)
{
    return lhs == rhs;
}

bool operator ==(const std::reference_wrapper<DrawableWorldObject>& lhs, const std::reference_wrapper<DrawableWorldObject>& rhs)
{
    return lhs == rhs;
}

std::list<Camera> World::GetCameras() const
{
    return cameras;
}

void World::SetCameras(const std::list<Camera>& value)
{
    cameras = value;
}

Camera& World::AddCamera(const Vec3& position)
{
    Camera cam;

    cam.SetPosition(position);
    cameras.push_back(cam);
    return cameras.back();
}

void World::RemoveCamera(Camera& cam)
{
    cameras.remove(cam);
}

void World::removeObject(DrawableWorldObject& obj)
{
    drawableObjects.remove(obj);
}

std::list<DrawableWorldObject>& World::GetDrawableObjects()
{
    return drawableObjects;
}

Vec3 World::GetWorldSize() const
{
    return worldSize;
}

void World::SetWorldSize(const Vec3& value)
{
    worldSize = value;
}

float World::GetScaleFactor() const
{
    return scaleFactor;
}

void World::SetScaleFactor(float value)
{
    scaleFactor = value;
}

World::World()
{
    
}

DrawableWorldObject& World::AddObject(const Mesh& obj, const Vec3& position)
{
    DrawableWorldObject drawableObject;

    drawableObject.SetMesh(obj);
    drawableObject.SetPosition(position);
    drawableObject.SetScale(scaleFactor);
    drawableObjects.push_back(drawableObject);
    return drawableObjects.back();
}

Light& World::AddLight(const Vec3& position, const Vec3& color)
{
    Light light;

    light.SetPosition(position);
    light.SetLightColor(color);
    lights.push_back(light);
    return lights.back();
}

std::list<Light>& World::GetLights()
{
    return lights;
}
