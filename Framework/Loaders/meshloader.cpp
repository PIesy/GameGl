#include "meshloader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, Texture> textureData;

Texture loadTexture(aiMaterial* material);

std::vector<GraphicsObject> MeshLoader::Load(const std::string& path)
{
    Assimp::Importer importer;
    std::vector<GraphicsObject> result;

    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate);

    for (unsigned m = 0; m < scene->mNumMeshes; m++)
    {
        bool setMaterial = false;
        RenderData data;
        aiMesh* mesh = scene->mMeshes[m];
        if (scene->HasMaterials())
            setMaterial = true;
        for (unsigned i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex v;

            v.coords = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            if (mesh->mNumUVComponents[0] != 0 && mesh->mTextureCoords[0] != nullptr)
                v.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            aiColor3D c;
            aiMaterial* material;
            aiString texname;

            scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, c);
            v.color = {c.r, c.g, c.b, 1.0f};
            data.vertices.push_back(v);
        }
        for (unsigned i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace& face = mesh->mFaces[i];
            //if (face.mNumIndices != 3)
                //Logger::Log("Nontriangular polygon found!");
            for (unsigned j = 0; j < face.mNumIndices; j++)
                data.indices.push_back(face.mIndices[j]);
        }    
        result.emplace_back(data);

        Texture tex = loadTexture(scene->mMaterials[mesh->mMaterialIndex]);
        result.back().setTexture(tex);
        if (mesh->mName.C_Str() != nullptr)
            result.back().setAttribute("name", mesh->mName.C_Str());
    }
    return result;
}

Texture loadTexture(aiMaterial* material)
{
    Texture tex;

    if (material->GetTextureCount(aiTextureType_DIFFUSE))
    {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        int x, y, n;
        std::string path = str.C_Str();
        path = path.substr(3);

        if (!textureData.count(path))
        {
            void* data = stbi_load(path.c_str(), &x, &y, &n, 0);
            tex.data = data;
            tex.height = y;
            tex.width = x;
            tex.componentsNum = n;
            tex.size = x * y * n;
            textureData.insert({path, tex});
        }
        tex = textureData[path];
    }
    return tex;
}
