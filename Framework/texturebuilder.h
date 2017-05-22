//
// Created by akann on 5/6/17.
//

#ifndef GAMEGL_TEXTUREBUILDER_H
#define GAMEGL_TEXTUREBUILDER_H


#include "../Graphics/texture.h"

class TextureBuilder
{
public:
    static Texture BuildTexture(TextureType type, TextureBindpoint target, unsigned width, unsigned height,
                                unsigned depth, unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps);
    static Texture BuildCubemap(TextureBindpoint target, unsigned width, unsigned height, unsigned count,
                                unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps);
    static Texture Build2DTexture(TextureBindpoint target, unsigned width, unsigned height, unsigned count,
                                unsigned channels, TexturePixelFormat pixelFormat, bool useMipmaps);
};


#endif //GAMEGL_TEXTUREBUILDER_H
