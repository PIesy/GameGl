#include "meshloader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../../Logger/logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, Texture> textureData;

Texture loadTexture(aiMaterial* material, StorageApi& api);
Mesh loadMesh(aiMesh* mesh, const aiScene* scene, StorageApi& api);
template<class T>
StoragePointer placeData(const std::vector<T>& data, StorageApi& api);

MeshLoader::MeshLoader(StorageApi& storage):api(storage)
{

}

std::vector<Mesh> MeshLoader::Load(const std::string& path)
{
    Assimp::Importer importer;
    std::vector<Mesh> result;

    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);

    for (unsigned m = 0; m < scene->mNumMeshes; m++)
        result.push_back(loadMesh(scene->mMeshes[m], scene, api));

    return result;
}

Mesh loadMesh(aiMesh* mesh, const aiScene* scene, StorageApi& api)
{
    static int meshId = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::string name;

    if (mesh->mName.length > 1)
        name = mesh->mName.C_Str();
    else
        name = std::to_string(meshId);

    for (unsigned i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;

        v.coords = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        v.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
        if (mesh->mNumUVComponents[0] != 0 && mesh->mTextureCoords[0] != nullptr)
            v.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        aiColor3D c;

        scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, c);
        v.color = {c.r, c.g, c.b, 1.0f};
        vertices.push_back(v);
    }
    for (unsigned i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3)
            Logger::Log("Nontriangular polygon found!");
        for (unsigned j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    MeshData data{placeData(vertices, api), placeData(indices, api)};
    MeshInfo info{vertices.size(), indices.size()};

    Mesh result{data, info, name};

    Texture tex = loadTexture(scene->mMaterials[mesh->mMaterialIndex], api);
    if (tex.info.channels != 0)
        result.AddTexture(tex);
    meshId++;
    return result;
}

template<class T>
StoragePointer placeData(const std::vector<T>& data, StorageApi& api)
{
    StorageDescriptor desc = api.Place(sizeof(T) * data.size(), data.data());
    return desc.pointer;
}

Texture loadTexture(aiMaterial* material, StorageApi& api)
{
    TextureLoader loader{api};
    Texture tex;

    if (material->GetTextureCount(aiTextureType_DIFFUSE))
    {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        std::string path = str.C_Str();
        path = path.substr(3);

        if (!textureData.count(path))
            textureData.emplace(path, loader.Load(path));
        tex = textureData[path];
    }
    return tex;
}
