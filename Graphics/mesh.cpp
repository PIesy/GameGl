//
// Created by akann on 4/26/17.
//

#include "mesh.h"

Mesh::Mesh(const MeshData& data, const MeshInfo& info, const std::string& id) : data(data), info(info), id(id) {}

Mesh::Mesh() {}

const MeshData& Mesh::GetData() const
{
    return data;
}

const MeshInfo& Mesh::GetInfo() const
{
    return info;
}

const std::string& Mesh::GetId() const
{
    return id;
}

const std::vector<Texture>& Mesh::GetTextures() const
{
    return textures;
}

void Mesh::AddTexture(Texture& texture)
{
    textures.emplace_back(texture);
}

void Mesh::SetId(const std::string& id)
{
    this->id = id;
}

void Mesh::AddTexture(Texture&& texture)
{
    textures.emplace_back(texture);
}

MeshData& Mesh::GetData()
{
    return data;
}
