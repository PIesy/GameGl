//
// Created by akann on 5/6/17.
//

#include "texturebuilder.h"
#include <algorithm>
#include <atomic>

constexpr char NAME_PREFIX[] = "\\|/generated_texture";
std::atomic_int id{0};

Texture TextureBuilder::BuildTexture(TextureType type, TextureBindpoint target, unsigned width, unsigned height, unsigned depth,
                             unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps)
{
    int current_id = id.fetch_add(1, std::memory_order_relaxed);
    Texture result;

    result.info.width = width;
    result.info.height = height;
    result.info.count = depth;
    result.info.type = type;
    result.info.targetPixelFormat = pixelFormat;
    result.info.channels = channels;
    result.info.name = std::string(NAME_PREFIX) + std::to_string(current_id);

    unsigned minSize = std::min(width, height);
    if (type == TextureType::Tex3D)
        minSize = std::min(minSize, depth);
    if (useMipmaps)
    {
        unsigned i = 0;
        while (minSize > 1)
        {
            minSize /= 2;
            i++;
        }
        result.info.mipmaps = i;
    }

    return result;
}

Texture TextureBuilder::BuildCubemap(TextureBindpoint target, unsigned width, unsigned height, unsigned count,
                                     unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps)
{
    int current_id = id.fetch_add(1, std::memory_order_relaxed);
    Texture result;

    result.info.width = width;
    result.info.height = height;
    result.info.count = count * 6;
    result.info.type = TextureType::Cubemap;
    result.info.target = target;
    result.info.targetPixelFormat = pixelFormat;
    result.info.channels = channels;
    result.info.name = std::string(NAME_PREFIX) + std::to_string(current_id);

    unsigned minSize = std::min(width, height);
    if (useMipmaps)
    {
        unsigned i = 0;
        while (minSize > 1)
        {
            minSize /= 2;
            i++;
        }
        result.info.mipmaps = i;
    }

    return result;
}

Texture TextureBuilder::Build2DTexture(TextureBindpoint target, unsigned width, unsigned height, unsigned count,
                                       unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps)
{
    int current_id = id.fetch_add(1, std::memory_order_relaxed);
    Texture result;

    result.info.width = width;
    result.info.height = height;
    result.info.count = count;
    result.info.type = TextureType::Tex2D;
    result.info.target = target;
    result.info.targetPixelFormat = pixelFormat;
    result.info.channels = channels;
    result.info.name = std::string(NAME_PREFIX) + std::to_string(current_id);

    unsigned minSize = std::min(width, height);
    if (useMipmaps)
    {
        unsigned i = 0;
        while (minSize > 1)
        {
            minSize /= 2;
            i++;
        }
        result.info.mipmaps = i;
    }

    return result;
}
