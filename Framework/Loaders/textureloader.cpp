//
// Created by akann on 4/25/17.
//

#include "textureloader.h"
#include <stb_image.h>
#include <algorithm>

TextureLoader::TextureLoader(StorageApi& storage):api(storage)
{

}

Texture TextureLoader::Load(const std::string& path)
{
    Texture tex;
    int height, width, components;
    void* data = stbi_load(path.c_str(), &height, &width, &components, 4);

    if (data != nullptr)
    {
        StorageDescriptor desc = api.Place(height * width * 4, data);
        stbi_image_free(data);
        TextureInfo parameters;

        parameters.height = height;
        parameters.width = width;
        parameters.channels = 4;
        parameters.name = path;

        tex.data.emplace_back(desc.pointer);
        tex.info = parameters;
    }

    return tex;
}

Texture TextureLoader::LoadCubemap(const std::vector<std::string>& files)
{
    Texture result;
    if (files.size() != 6)
        return result;

    std::vector<Texture> faces;
    for (const std::string& str : files)
        faces.push_back(Load(str));

    result = faces[0];
    result.info.type = TextureType::Cubemap;
    result.info.count = 6;
    for (unsigned i = 1; i < faces.size(); ++i)
        result.data.push_back(faces[i].data[0]);
    result.info.name += ".cubemap";
    return result;
}
