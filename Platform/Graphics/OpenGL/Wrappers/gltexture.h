#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "glbindings.h"
#include "../../../../Helpers/helpers.h"
#include <vector>

namespace gl {

    enum class TextureType : GLenum
    {
        Tex2d = GL_TEXTURE_2D,
        Tex2dArray = GL_TEXTURE_2D_ARRAY,
        Tex1d = GL_TEXTURE_1D,
        Tex1dArray = GL_TEXTURE_1D_ARRAY,
        TexCube = GL_TEXTURE_CUBE_MAP,
        Tex3d = GL_TEXTURE_3D
    };

    enum class SourceFormat : GLenum
    {
        RGBA = GL_RGBA,
        RGB = GL_RGB,
        BGR = GL_BGR,
        BGRA = GL_BGRA,
        Depth = GL_DEPTH_COMPONENT,
        Stencil = GL_STENCIL_INDEX
    };

    enum class SourcePixelSize : GLenum
    {
        Byte = GL_UNSIGNED_BYTE,
        Short = GL_UNSIGNED_SHORT,
        Float = GL_FLOAT
    };

    enum class InternalFormat : GLenum
    {
        RGBA8 = GL_RGBA8,
        RGBA16F = GL_RGBA16F,
        RGBA32F = GL_RGBA32F,
        RGB8 = GL_RGB8,
        RGB16F = GL_RGB16F,
        RGB32F = GL_RGB32F,
        Depth16 = GL_DEPTH_COMPONENT16,
        Depth24 = GL_DEPTH_COMPONENT24,
        Depth32 = GL_DEPTH_COMPONENT32F,
        Depth24Stencil8 = GL_DEPTH24_STENCIL8,
        Depth32Stencil8 = GL_DEPTH32F_STENCIL8,
        Stencil = GL_STENCIL_INDEX8
    };

    enum class FilterType : GLint
    {
        Nearest = GL_NEAREST,
        MipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        Linear = GL_LINEAR,
        MipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class WrappingType : GLint
    {
        ClampToEdge = GL_CLAMP_TO_EDGE,
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToBorder = GL_CLAMP_TO_BORDER
    };

    enum class CompareFunction : GLint
    {
        Lequal = GL_LEQUAL,
        Lesser = GL_LESS,
        Gequal = GL_GEQUAL,
        Greater = GL_GREATER,
        Equal = GL_EQUAL,
        Nequal = GL_NOTEQUAL
    };

    enum class CompareMode : GLint { ReferenceToTexture = GL_COMPARE_REF_TO_TEXTURE };


    class GlTexture
    {
        GLuint texture = 0;
        TextureType type;
        unsigned width;
        unsigned height;
        unsigned depth;
    public:
        GlTexture();
        GlTexture(TextureType type);
        GlTexture(GLuint id, TextureType type);
        GlTexture(const GlTexture&) = delete;
        GlTexture& operator=(const GlTexture&) = delete;
        GlTexture(GlTexture&& src);
        GlTexture& operator=(GlTexture&& src);
        ~GlTexture();

        void Bind(int textureUnit = 0);
        void Allocate(InternalFormat format, unsigned width, unsigned height, unsigned depth, unsigned mipmapsCount);
        void Load(SourceFormat format, SourcePixelSize pixelSize, const void* data, unsigned int mipmapLevel);
        void Load(SourceFormat format, SourcePixelSize pixelSize, unsigned xOffset, unsigned yOffset, unsigned zOffset, const void* data, unsigned int levels,
                          unsigned int mipmapLevel);
        void SetAnisotropicFiltering(int level);
        void SetMinFilter(FilterType type);
        void SetMagFilter(FilterType type);
        void SetSWrapping(WrappingType type);
        void SetTWrapping(WrappingType type);
        void SetRWrapping(WrappingType type);
        void SetComparisonMode(CompareFunction function, CompareMode mode);

        GLuint GetId() const;
    };

}

#endif // GLTEXTURE_H
