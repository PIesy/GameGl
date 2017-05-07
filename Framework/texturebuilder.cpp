//
// Created by akann on 5/6/17.
//

#include "texturebuilder.h"
#include <algorithm>

Texture
TextureBuilder::BuildTexture(TextureType type, TextureBindpoint target, unsigned width, unsigned height, unsigned depth,
                             unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps)
{
    Texture result;

    result.info.width = width;
    result.info.height = height;
    result.info.count = depth;
    result.info.type = type;
    result.info.targetPixelFormat = pixelFormat;
    result.info.channels = channels;

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
