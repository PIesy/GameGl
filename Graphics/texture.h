#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>
#include "../Helpers/storagepointer.h"

enum class TextureType { Tex1D, Tex2D, Tex3D, Cubemap };
enum class TextureBindpoint { Color, Depth, Stencil, DepthStencil };
enum class TexturePixelFormat { Float8, Float16, Float24, Float32 };

struct TextureInfo
{
    unsigned width = 0;
    unsigned height = 0;
    unsigned channels = 0;
    unsigned count = 1;
    unsigned mipmaps = 0;
    TextureType type = TextureType::Tex2D;
    TextureBindpoint target = TextureBindpoint::Color;
    TexturePixelFormat pixelFormat = TexturePixelFormat::Float8;
    std::string name;
};

enum class TextureWrapping { Clamp, None, Repeat, MirroredRepeat };
enum class TextureSampling { Linear, Nearest };

struct TextureParameters
{
    TextureWrapping wrapping = TextureWrapping::Clamp;
    TextureSampling sampling = TextureSampling::Linear;
    unsigned anisotropicFiltering = 0;
};


struct Texture
{
    std::vector<StoragePointer> data;
    TextureInfo info;
    TextureParameters parameters;
};

#endif // TEXTURE_H
