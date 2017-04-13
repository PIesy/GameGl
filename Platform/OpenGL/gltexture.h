#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "glbindings.h"
#include "../../Helpers/helpers.h"
#include <vector>

namespace gl
{

class GlTexture;

enum class TextureType:GLenum {Tex2d = GL_TEXTURE_2D,
    Tex1d = GL_TEXTURE_1D,
    TexCube = GL_TEXTURE_CUBE_MAP};

class GlTextureFactory
{
public:
    static std::vector<GlTexture> CreateTextures(int count, TextureType type)
    {
        std::vector<GlTexture> result;
        std::vector<GLuint> ids(count);

        gl::texture::generate(count, ids.data(), integral(type));
        for (GLuint& id : ids)
            result.emplace_back(id, type);
        return result;
    }
};

enum class TextureFormat:GLenum {RGBA = GL_RGBA,
                          RGB = GL_RGB,
                          BGR = GL_BGR,
                          BGRA = GL_BGRA};

enum class InternalFormat:GLenum {RGBA8 = GL_RGBA8,
                    RGB8 = GL_RGB8,
                    Depth16 = GL_DEPTH_COMPONENT16,
                    Depth24 = GL_DEPTH_COMPONENT24,
                    Depth32 = GL_DEPTH_COMPONENT32};

enum class FilterType:GLint {Nearest = GL_NEAREST, Linear = GL_LINEAR};

enum class WrappingType:GLint {ClampToEdge = GL_CLAMP_TO_EDGE,
                        Repeat = GL_REPEAT};

enum class CompareFunction:GLint {Lequal = GL_LEQUAL,
                            Lesser = GL_LESS,
                            Gequal = GL_GEQUAL,
                            Greater = GL_GREATER,
                            Equal = GL_EQUAL,
                            Nequal = GL_NOTEQUAL};

enum class CompareMode:GLint {ReferenceToTexture = GL_COMPARE_REF_TO_TEXTURE};


class GlTexture
{
    GLuint texture = 0;
    TextureType type;
    unsigned width;
    unsigned height;
public:
    GlTexture();
    GlTexture(TextureType type);
    GlTexture(GLuint id, TextureType type);
    GlTexture(const GlTexture&) = delete;
    GlTexture& operator =(const GlTexture&) = delete;
    GlTexture(GlTexture&& src);
    GlTexture& operator =(GlTexture&& src);
    ~GlTexture();

    void Bind(int textureUnit = 0);
    void Allocate(InternalFormat format);
    void Load(TextureFormat format, void* data, int mipmapLevel = 0);
    void setMinFilter(FilterType type);
    void setMagFilter(FilterType type);
    void setSWrapping(WrappingType type);
    void setTWrapping(WrappingType type);
    void setSize(unsigned width, unsigned height);
    void setComparisonMode(CompareFunction function, CompareMode mode);
    GLuint getId() const;
};

}

#endif // GLTEXTURE_H
