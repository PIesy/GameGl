//
// Created by akann on 6/6/17.
//
#ifndef GAMEGL_RESOURCES_HELPER_H
#define GAMEGL_RESOURCES_HELPER_H

#include "Storage/storageapi.h"
#include "Graphics/mesh.h"
#include "Framework/material.h"
#include <unordered_map>

enum class Meshes {Sofa, Sphere, Pistol, Cube, Table};
enum class Materials {Gold, Iron, Plastic, Stone, Grass, Fabric, WoodenFloor, Aluminum};

class ResourcesHelper
{
    std::unordered_map<int, std::vector<Mesh>> meshes;
    std::unordered_map<int, Material> textures;
    void loadTextures(StorageApi& api);
    void loadMeshes(StorageApi& api);
public:
    void Init(StorageApi& api);
    std::vector<Mesh>& GetMesh(Meshes id);
    Material GetMaterial(Materials id);
};


#endif //GAMEGL_RESOURCES_HELPER_H
