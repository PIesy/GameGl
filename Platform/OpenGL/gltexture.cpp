#include "gltexture.h"
#include "../../Logger/logger.h"

void gl::GlTexture::setSize(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;
}

void gl::GlTexture::setComparisonMode(gl::CompareFunction function, gl::CompareMode mode)
{
    gl::texture::setParameter(texture, GL_TEXTURE_COMPARE_FUNC, integral(function));
    gl::texture::setParameter(texture, GL_TEXTURE_COMPARE_MODE, integral(mode));
}

GLuint gl::GlTexture::getId() const
{
    return texture;
}

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
    gl::texture::setActiveTexture(GL_TEXTURE0 + textureUnit);
    gl::texture::bind(integral(type), texture);
}

void gl::GlTexture::Load(gl::TextureFormat format, void* data, int mipmapLevel)
{
    switch (type)
    {
    case TextureType::Tex2d:
        gl::texture::loadImage(texture, mipmapLevel, 0, 0, width, height, integral(format), GL_UNSIGNED_BYTE, data);
        break;
    default:
        Logger::Log("Not yet implemented");
        break;
    }
}

void gl::GlTexture::setMinFilter(gl::FilterType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_MIN_FILTER, integral(type));
}

void gl::GlTexture::setMagFilter(gl::FilterType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_MAG_FILTER, integral(type));
}

void gl::GlTexture::setSWrapping(gl::WrappingType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_WRAP_S, integral(type));
}

void gl::GlTexture::setTWrapping(gl::WrappingType type)
{
    gl::texture::setParameter(texture, GL_TEXTURE_WRAP_T, integral(type));
}

gl::GlTexture::GlTexture()
{

}

void gl::GlTexture::Allocate(gl::InternalFormat format)
{
    switch (type)
    {
        case TextureType::Tex2d:
            gl::texture::allocateStorage(texture, 1, integral(format), width, height);
            break;
        default:
            Logger::Log("Not yet implemented");
            break;
    }
}

gl::GlTexture& gl::GlTexture::operator=(gl::GlTexture&& src)
{
    width = src.width;
    height = src.height;
    texture = src.texture;
    type = src.type;
    src.texture = 0;
    return *this;
}

gl::GlTexture::GlTexture(gl::GlTexture&& src)
{
    *this = std::move(src);
}

gl::GlTexture::~GlTexture()
{
    gl::texture::erase(1, &texture);
}
