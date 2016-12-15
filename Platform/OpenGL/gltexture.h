#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "Platform/OpenGL/glbindings.h"
#include <vector>

namespace gl
{

class GlTexture;

class GlTextureFactory
{
public:
    static std::vector<GlTexture> CreateTextures(int count)
    {
        std::vector<GlTexture> result;
        std::vector<GLuint> ids(count);

        gl::texture::generate(count, ids.data());
        for (GLuint& id : ids)
            result.emplace_back(id);
        return result;
    }
};

enum class TextureType {Tex2d = GL_TEXTURE_2D,
                        Tex1d = GL_TEXTURE_1D,
                        TexCube = GL_TEXTURE_CUBE_MAP};

enum class TextureFormat {RGBA = GL_RGBA,
                          RGB = GL_RGB,
                          BGR = GL_BGR,
                          BGRA = GL_BGRA,
                          Depth = GL_DEPTH_COMPONENT,
                          Depth16 = GL_DEPTH_COMPONENT16,
                          Depth24 = GL_DEPTH_COMPONENT24,
                          Depth32 = GL_DEPTH_COMPONENT32};
enum class FilterType {Nearest = GL_NEAREST, Linear = GL_LINEAR};

enum class WrappingType {ClampToEdge = GL_CLAMP_TO_EDGE};

enum class CompareFunction {Lequal = GL_LEQUAL,
                            Lesser = GL_LESS,
                            Gequal = GL_GEQUAL,
                            Greater = GL_GREATER,
                            Equal = GL_EQUAL,
                            Nequal = GL_NOTEQUAL};

enum class CompareMode {ReferenceToTexture = GL_COMPARE_REF_TO_TEXTURE};


class GlTexture
{
    GLuint id;
    TextureType type;
    TextureFormat format;
    unsigned width;
    unsigned height;
public:
    GlTexture();
    GlTexture(GLuint id);
    void Bind();
    void Load(TextureFormat format, void* data = nullptr, int mipmapLevel = 0);
    void setMinFilter(FilterType type);
    void setMagFilter(FilterType type);
    void setSWrapping(WrappingType type);
    void setTWrapping(WrappingType type);
    void setType(const TextureType& value);
    void setFormat(const TextureFormat& value);
    void setSize(unsigned width, unsigned height);
    void setComparison(CompareFunction function, CompareMode mode);
    GLuint getId() const;
};

}

#endif // GLTEXTURE_H
