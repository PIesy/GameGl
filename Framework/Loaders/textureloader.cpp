//
// Created by akann on 4/25/17.
//

#include "textureloader.h"
#include <stb_image.h>
#include <algorithm>

TextureLoader::TextureLoader(StorageApi& storage):api(storage)
{

}

Texture TextureLoader::Load(const std::string& path, bool useHDR)
{
    Texture tex;
    int height, width, components;
    void* data = nullptr;
    if (!useHDR)
        data = stbi_load(path.c_str(), &width, &height, &components, 4);
    else
        data = stbi_loadf(path.c_str(), &width, &height, &components, 4);

    if (data != nullptr)
    {
        size_t pixelSize = useHDR ? sizeof(float) : 1;
        StorageDescriptor desc = api.Get(std::hash<std::string>{}(path));
        if (!desc.size)
            desc = api.Place(height * width * 4 * pixelSize, data, std::hash<std::string>{}(path));
        stbi_image_free(data);
        TextureInfo parameters;

        parameters.height = height;
        parameters.width = width;
        parameters.channels = 4;
        parameters.name = path;
        if (useHDR)
        {
            parameters.sourcePixelFormat = TexturePixelFormat::Float32;
            parameters.targetPixelFormat = TexturePixelFormat::Float32;
        }

        tex.data.emplace_back(desc.pointer);
        tex.info = parameters;
    }

    return tex;
}

Texture TextureLoader::LoadCubemap(const std::array<std::string, 6>& files, bool useHDR)
{
    Texture result;

    std::vector<Texture> faces;
    for (const std::string& str : files)
        faces.push_back(Load(str, useHDR));

    result = faces[0];
    result.info.type = TextureType::Cubemap;
    result.info.count = 6;
    for (unsigned i = 1; i < faces.size(); ++i)
        result.data.push_back(faces[i].data[0]);
    for (const std::string& str : files)
        result.info.name += str;
    result.info.name += ".cubemap";
    return result;
}

Texture TextureLoader::LoadArray(const std::vector<std::string>& files, bool useHDR)
{
    Texture result;

    std::vector<Texture> array;
    for (const std::string& str : files)
        array.push_back(Load(str, useHDR));

    result = array[0];
    result.info.count = files.size();
    for (unsigned i = 1; i < array.size(); ++i)
        result.data.push_back(array[i].data[0]);
    for (const std::string& str : files)
        result.info.name += str;
    result.info.name += ".array";
    return result;
}
