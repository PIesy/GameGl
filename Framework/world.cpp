#include "world.h"
#include "material.h"

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

DrawableWorldObject& World::AddObject(const Mesh& obj, const Vec3& position, const Material& overrideMaterial)
{
    return AddObject(std::vector<Mesh>{obj}, position, overrideMaterial);
}

DrawableWorldObject& World::AddObject(const std::vector<Mesh>& obj, const Vec3& position, const Material& overrideMaterial)
{
    DrawableWorldObject object;

    object.SetMeshes(obj);
    object.SetPosition(position);
    if (!overrideMaterial.texture.empty())
    {
        for (Mesh& mesh : object.GetMeshes())
        {
            mesh.SetMaterialProperties(overrideMaterial.properties);
            mesh.SetTextures(overrideMaterial.texture);
        }
    }
    drawableObjects.push_back(std::move(object));
    return drawableObjects.back();
}

Light& World::AddPointLight(const Vec3& position, const Vec3& color)
{
    Light light{LightType::Point};

    light.SetPosition(position);
    light.SetLightColor(color);
    pointLights.push_back(std::move(light));
    return pointLights.back();
}

Light& World::AddSpotlight(const Vec3& position, const Vec3& color, const Vec3& direction, float angle)
{
    Light light{LightType::Spotlight};

    light.SetPosition(position);
    light.SetLightColor(color);
    light.SetDirection(direction);
    light.SetAngle(angle);
    spotlights.push_back(std::move(light));
    return spotlights.back();
}

Light& World::AddDirectionalLight(const Vec3& color, const Vec3& direction)
{
    Light light{LightType::Directional};
    Vec3 position = -glm::normalize(direction) * (glm::length(worldSize) / 2.0f) + Vec3{worldSize.x / 2, worldSize.y / 2, worldSize.z / 2};

    light.SetPosition(position);
    light.SetLightColor(color);
    light.SetDirection(direction);
    directionalLights.push_back(std::move(light));
    return directionalLights.back();
}

std::vector<Light>& World::GetPointLights()
{
    return pointLights;
}

std::vector<Light>& World::GetDirectionalLights()
{
    return directionalLights;
}

std::vector<Light>& World::GetSpotights()
{
    return spotlights;
}
