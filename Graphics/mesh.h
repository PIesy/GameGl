//
// Created by akann on 4/26/17.
//

#ifndef GAMEGL_MESH_H
#define GAMEGL_MESH_H

#include "../Helpers/storagepointer.h"
#include "graphicsclasses.h"
#include "texture.h"

struct MeshData
{
    TypedStoragePointer<Vertex> vertices;
    TypedStoragePointer<unsigned> indices;
};

struct MeshInfo
{
    unsigned long vertexCount = 0;
    unsigned long indexCount = 0;
};

class Mesh
{
    MeshData data;
    MeshInfo info;
    std::string id;
    std::vector<Texture> textures;
public:
    Mesh();
    Mesh(const MeshData& data, const MeshInfo& info, const std::string& id);

    const MeshData& GetData() const;

    MeshData& GetData();

    const MeshInfo& GetInfo() const;

    const std::string& GetId() const;

    const std::vector<Texture>& GetTextures() const;

    void SetId(const std::string& id);

    void AddTexture(Texture& texture);

    void AddTexture(Texture&& texture);
};


#endif //GAMEGL_MESH_H
