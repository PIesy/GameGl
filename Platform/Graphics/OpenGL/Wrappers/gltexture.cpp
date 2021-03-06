#include "gltexture.h"
#include "../../../../Logger/logger.h"

const auto logger = Logger::GetLogger(getClassName<gl::GlTexture>());

void gl::GlTexture::SetComparisonMode(gl::CompareFunction function, gl::CompareMode mode)
{
    gl::texture::setParameter(texture, GL_TEXTURE_COMPARE_FUNC, integral(function));
    gl::texture::setParameter(texture, GL_TEXTURE_COMPARE_MODE, integral(mode));
}

GLuint gl::GlTexture::GetId() const
{
    return texture;
}

gl::GlTexture::GlTexture() = default;

gl::GlTexture::GlTexture(TextureType type)
{
    gl::texture::generate(1, &texture, integral(type));
    this->type = type;
}

gl::GlTexture::GlTexture(GLuint texture, TextureType type)
{
    this->texture = texture;
    this->type = type;
}

void gl::GlTexture::Bind(int textureUnit)
{
    if (texture != 0)
    {
        gl::texture::setActiveTexture(GL_TEXTURE0 + textureUnit);
        gl::texture::bind(integral(type), texture);
    }
}

void gl::GlTexture::Load(SourceFormat format, SourcePixelSize pixelSize, const void* data, unsigned int mipmapLevel)
{
    switch (type)
    {
        case TextureType::Tex2d:
            gl::texture::loadImage(texture, mipmapLevel, 0, 0, width, height, integral(format), integral(pixelSize), data);
            break;
        case TextureType::Tex3d:
        case TextureType::Tex2dArray:
        case TextureType::TexCube:
            gl::texture::loadImage(texture, mipmapLevel, 0, 0, 0, width, height, depth,
                                   integral(format), integral(pixelSize), data);
            break;
        default:
            logger.LogError("Not yet implemented");
            break;
    }
}


void gl::GlTexture::Load(SourceFormat format, SourcePixelSize pixelSize, unsigned xOffset, unsigned yOffset, unsigned zOffset,
                         const void* data, unsigned int levels, unsigned int mipmapLevel)
{
    switch (type)
    {
        case TextureType::Tex2d:
            gl::texture::loadImage(texture, mipmapLevel, xOffset, yOffset,
                                   width, height, integral(format), integral(pixelSize), data);
            break;
        case TextureType::Tex3d:
        case TextureType::Tex2dArray:
        case TextureType::TexCube:
            gl::texture::loadImage(texture, mipmapLevel, xOffset, yOffset, zOffset, width, height, levels,
                                   integral(format), integral(pixelSize), data);
            break;
        default:
            logger.LogError("Not yet implemented");
            break;
    }
}

void gl::GlTexture::SetMinFilter(gl::FilterType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_MIN_FILTER, integral(type));
}

void gl::GlTexture::SetMagFilter(gl::FilterType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_MAG_FILTER, integral(type));
}

void gl::GlTexture::SetSWrapping(gl::WrappingType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_WRAP_S, integral(type));
}

void gl::GlTexture::SetTWrapping(gl::WrappingType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_WRAP_T, integral(type));
}

void gl::GlTexture::SetRWrapping(gl::WrappingType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_WRAP_R, integral(type));
}

void gl::GlTexture::Allocate(gl::InternalFormat format, unsigned width, unsigned height, unsigned depth,
                             unsigned mipmapsCount)
{
    this->width = width;
    this->height = height;
    this->depth = depth;
    switch (type)
    {
        case TextureType::Tex2d:
        case TextureType::Tex1dArray:
        case TextureType::TexCube:
            gl::texture::allocateStorage(texture, mipmapsCount + 1, integral(format), width, height);
            break;
        case TextureType::Tex3d:
        case TextureType::Tex2dArray:
        case TextureType::TexCubeArray:
            gl::texture::allocateStorage(texture, mipmapsCount + 1, integral(format), width, height, depth);
            break;
        default:
            logger.LogError("Cant create that texture type");
            break;
    }
}

gl::GlTexture& gl::GlTexture::operator=(gl::GlTexture&& src) noexcept
{
    gl::texture::erase(1, &texture);
    width = src.width;
    height = src.height;
    depth = src.depth;
    texture = src.texture;
    type = src.type;
    src.texture = 0;
    return *this;
}

gl::GlTexture::GlTexture(gl::GlTexture&& src) noexcept
{
    *this = std::move(src);
}

gl::GlTexture::~GlTexture()
{
    gl::texture::erase(1, &texture);
}

void gl::GlTexture::SetAnisotropicFiltering(int level)
{
    gl::texture::setParameter(texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
}
