//
// Created by akann on 3/19/17.
//

#ifndef GAMEGL_GLFRAMEBUFFER_H
#define GAMEGL_GLFRAMEBUFFER_H

#include <vector>
#include "glbindings.h"
#include "gltexture.h"
#include "globject.h"
#include "glrenderbuffer.h"

namespace gl {

    constexpr GLuint DEFAULT_FRAMEBUFFER = 0;

    enum class FrameBufferTarget: GLenum
    {
        Read = GL_READ_FRAMEBUFFER,
        Draw = GL_DRAW_FRAMEBUFFER,
        Both = GL_FRAMEBUFFER
    };

    enum class FrameBufferAttachment: GLenum
    {
        Color = GL_COLOR_ATTACHMENT0,
        Depth = GL_DEPTH_ATTACHMENT,
        Stencil = GL_STENCIL_ATTACHMENT,
        DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
        None = GL_NONE
    };

    constexpr FrameBufferAttachment operator+(const FrameBufferAttachment& a, int b)
    {
        return static_cast<FrameBufferAttachment>(integral(a) + b);
    }

    enum class BufferBit : GLbitfield
    {
        Color = GL_COLOR_BUFFER_BIT,
        Depth = GL_DEPTH_BUFFER_BIT,
        Stencil = GL_STENCIL_BUFFER_BIT,
        None = 0
    };

    constexpr BufferBit operator|(const BufferBit& a, const BufferBit& b)
    {
        return static_cast<BufferBit>(integral(a) | integral(b));
    }

    constexpr BufferBit& operator|=(BufferBit& a, const BufferBit& b)
    {
        a = a | b;
        return a;
    }

    enum class FrameBufferStatus: GLenum
    {
        Complete = GL_FRAMEBUFFER_COMPLETE,
        Undefined = GL_FRAMEBUFFER_UNDEFINED,
        IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
        MissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
        IncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
        IncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
        Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,
        IncompleteMultisample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
        IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
    };

    class GlFrameBuffer : public GlObject
    {
        GLuint frameBuffer = 0;
    public:
        GlFrameBuffer();
        explicit GlFrameBuffer(unsigned id);
        GlFrameBuffer(const GlFrameBuffer&) = delete;
        GlFrameBuffer& operator =(const GlFrameBuffer& src) = delete;
        GlFrameBuffer(GlFrameBuffer&& src) noexcept;
        GlFrameBuffer& operator =(GlFrameBuffer&& src) noexcept;
        ~GlFrameBuffer() override;

        void Bind(FrameBufferTarget target = FrameBufferTarget::Both);
        void BindRenderBuffer(FrameBufferAttachment target, GlRenderBuffer& renderBuffer);
        void SetDrawBuffers(const std::vector<FrameBufferAttachment>& drawBuffers);
        void SetTextureToAttachment(FrameBufferAttachment target, GlTexture& texture, int level);
        void Blit(GlFrameBuffer& source, int srcX0, int srcY0, int srcX1, int srcY1,
                  int targetX0, int targetY0, int targetX1, int targetY1, BufferBit buffers, FilterType filterType);
        FrameBufferStatus GetStatus(FrameBufferTarget target);
        GLuint GetId() const override;
    };

}


#endif //GAMEGL_GLFRAMEBUFFER_H
