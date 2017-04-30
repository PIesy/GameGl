//
// Created by akann on 4/25/17.
//

#include "textureloader.h"
#include <stb_image.h>

TextureLoader::TextureLoader(StorageApi& storage):api(storage)
{

}

Texture TextureLoader::Load(const std::string& path)
{
    Texture tex;
    int height, width, components;
    void* data = stbi_load(path.c_str(), &height, &width, &components, 0);

    if (data != nullptr)
    {
        StorageDescriptor desc = api.Place(height * width * components, data);
        stbi_image_free(data);
        TextureInfo parameters;

        parameters.height = height;
        parameters.width = width;
        parameters.channels = components;
        parameters.name = path;

        tex.data.emplace_back(desc.pointer);
        tex.info = parameters;
    }

    return tex;
}
