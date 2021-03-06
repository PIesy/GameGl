//
// Created by akann on 4/29/17.
//

#include "gltexturefactory.h"
#include "../../../Logger/logger.h"

const auto logger = Logger::GetLogger(getClassName<gl::GlTextureFactory>());

gl::GlTexture gl::GlTextureFactory::BuildTexture(const TextureInfo& parameters, const std::vector<StoragePointer>& pointers)
{
    switch(parameters.type)
    {
        case ::TextureType::Tex2D:
        case ::TextureType::Cubemap:
            return build2dTexture(parameters, pointers);
        default:
            logger.LogError("Cant create that texture yet");
            return gl::GlTexture();
    }
}

gl::GlTexture gl::GlTextureFactory::build2dTexture(const TextureInfo& parameters, const std::vector<StoragePointer>& pointers)
{
    gl::GlTexture result;

    if (parameters.type == ::TextureType::Cubemap)
    {
        if (parameters.count / 6 == 1)
            result = gl::GlTexture{TextureType::TexCube};
        else if (parameters.count / 6 > 1)
            result = gl::GlTexture{TextureType::TexCubeArray};
    }
    else if (parameters.count == 1)
        result = gl::GlTexture{TextureType::Tex2d};
    else
        result = gl::GlTexture{TextureType::Tex2dArray};

    result.Allocate(decodeFormat(parameters), parameters.width, parameters.height, parameters.count, parameters.mipmaps);

    gl::SourcePixelSize pixelSize;
    switch (parameters.sourcePixelFormat)
    {
        case TexturePixelFormat::Float8:
            pixelSize = gl::SourcePixelSize::Byte;
            break;
        case TexturePixelFormat::Float16:
            pixelSize = gl::SourcePixelSize::Short;
            break;
        case TexturePixelFormat::Float24:
        case TexturePixelFormat::Float32:
            pixelSize = gl::SourcePixelSize::Float;
            break;
    }

    unsigned i = 0;
    for (const StoragePointer& p : pointers)
    {
        if (p.IsValid())
        {
            switch(parameters.target)
            {
                case TextureBindpoint::Color:
                    if (parameters.count == 1)
                    {
                        if (parameters.channels == 3)
                            result.Load(gl::SourceFormat::RGB, pixelSize, p, 0);
                        if (parameters.channels == 4)
                            result.Load(gl::SourceFormat::RGBA, pixelSize, p, 0);
                    }
                    else
                    {
                        if (parameters.channels == 3)
                            result.Load(gl::SourceFormat::RGB, pixelSize, 0, 0, i, p, 1, 0);
                        if (parameters.channels == 4)
                            result.Load(gl::SourceFormat::RGBA, pixelSize, 0, 0, i, p, 1, 0);
                    }
                    break;
                case TextureBindpoint::Depth:
                    if (parameters.count == 1)
                        result.Load(gl::SourceFormat::Depth, pixelSize, p, 0);
                    else
                        result.Load(gl::SourceFormat::Depth, pixelSize, 0, 0, i, p, 1, 0);
                    break;
                case TextureBindpoint::Stencil:
                    if (parameters.count == 1)
                        result.Load(gl::SourceFormat::Stencil, pixelSize, p, 0);
                    else
                        result.Load(gl::SourceFormat::Stencil, pixelSize, 0, 0, i, p, 1, 0);
                    break;
                case TextureBindpoint::DepthStencil:
                    break;
            }
        }
        i++;
    }
    return result;
}

gl::InternalFormat gl::GlTextureFactory::decodeFormat(const TextureInfo& parameters)
{
    switch (parameters.target)
    {
        case TextureBindpoint::Color:
            switch (parameters.targetPixelFormat)
            {
                case TexturePixelFormat::Float8:
                    if (parameters.channels == 3)
                        return gl::InternalFormat::RGB8;
                    if (parameters.channels == 4)
                        return gl::InternalFormat::RGBA8;
                    break;
                case TexturePixelFormat::Float16:
                    if (parameters.channels == 3)
                        return gl::InternalFormat::RGB16F;
                    if (parameters.channels == 4)
                        return gl::InternalFormat::RGBA16F;
                    break;
                case TexturePixelFormat::Float24:
                    logger.LogWarning("Unsupported texture format RGB(A) 24 bit per channel using 32 bit instead");
                case TexturePixelFormat::Float32:
                    if (parameters.channels == 3)
                        return gl::InternalFormat::RGB32F;
                    if (parameters.channels == 4)
                        return gl::InternalFormat::RGBA32F;
                    break;
            }
            break;
        case TextureBindpoint::Depth:
            switch (parameters.targetPixelFormat)
            {
                case TexturePixelFormat::Float8:
                    logger.LogWarning("Unsupported texture format Depth 8 bit using 16 bit instead");
                case TexturePixelFormat::Float16:
                    return gl::InternalFormat::Depth16;
                case TexturePixelFormat::Float24:
                    return gl::InternalFormat::Depth24;
                case TexturePixelFormat::Float32:
                    return gl::InternalFormat::Depth32;
            }
        case TextureBindpoint::Stencil:
            return gl::InternalFormat::Stencil;
        case TextureBindpoint::DepthStencil:
            return gl::InternalFormat::Depth24Stencil8;
    }
    return gl::InternalFormat::RGBA8;
}
