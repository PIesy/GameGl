#include "gltexture.h"
#include "Helpers/helpers.h"
#include "Logger/logger.h"

class RAIITextureBinding
{
    GLuint texture;
    gl::TextureType type;
public:
    RAIITextureBinding(GLuint texture, gl::TextureType type)
    {
        this->type = type;
        this->texture = texture;
        gl::texture::bind(integral(type), texture);
    }
    ~RAIITextureBinding()
    {
        gl::texture::bind(integral(type), 0);
    }
};

void gl::GlTexture::setType(const TextureType& value)
{
    type = value;
}

void gl::GlTexture::setFormat(const TextureFormat& value)
{
    format = value;
}

void gl::GlTexture::setSize(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;
}

void gl::GlTexture::setComparison(gl::CompareFunction function, gl::CompareMode mode)
{
    RAIITextureBinding t(id, type);
    gl::texture::setParameter(integral(type), GL_TEXTURE_COMPARE_FUNC, integral(function));
    gl::texture::setParameter(integral(type), GL_TEXTURE_COMPARE_MODE, integral(mode));
}

GLuint gl::GlTexture::getId() const
{
    return id;
}

gl::GlTexture::GlTexture()
{
    gl::texture::generate(1, &id);
}

gl::GlTexture::GlTexture(GLuint id)
{
    this->id = id;
}

void gl::GlTexture::Bind()
{
    gl::texture::bind(integral(type), id);
}

void gl::GlTexture::Load(gl::TextureFormat format, void* data, int mipmapLevel)
{
    RAIITextureBinding t(id, type);
    switch (type)
    {
    case TextureType::Tex2d:
        gl::texture::load2Dimage(integral(TextureType::Tex2d), mipmapLevel, integral(format), width, height, integral(format), GL_UNSIGNED_BYTE, data);
        break;
    default:
        Logger::Log("Not yet implemented");
        break;
    }
}

void gl::GlTexture::setMinFilter(gl::FilterType type)
{
    RAIITextureBinding t(id, this->type);
    gl::texture::setParameter(integral(this->type), GL_TEXTURE_MIN_FILTER, integral(type));
}

void gl::GlTexture::setMagFilter(gl::FilterType type)
{
    RAIITextureBinding t(id, this->type);
    gl::texture::setParameter(integral(this->type), GL_TEXTURE_MAG_FILTER, integral(type));
}

void gl::GlTexture::setSWrapping(gl::WrappingType type)
{
    RAIITextureBinding t(id, this->type);
    gl::texture::setParameter(integral(this->type), GL_TEXTURE_WRAP_S, integral(type));
}

void gl::GlTexture::setTWrapping(gl::WrappingType type)
{
    RAIITextureBinding t(id, this->type);
    gl::texture::setParameter(integral(this->type), GL_TEXTURE_WRAP_T, integral(type));
}
