//
// Created by akann on 3/19/17.
//

#ifndef GAMEGL_GLFRAMEBUFFER_H
#define GAMEGL_GLFRAMEBUFFER_H

#include <vector>
#include "glbindings.h"
#include "gltexture.h"
#include "globject.h"

namespace gl {

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
        GlFrameBuffer(unsigned id);
        GlFrameBuffer(const GlFrameBuffer&) = delete;
        GlFrameBuffer& operator =(const GlFrameBuffer& src) = delete;
        GlFrameBuffer(GlFrameBuffer&& src);
        GlFrameBuffer& operator =(GlFrameBuffer&& src);
        ~GlFrameBuffer();

        void Bind(FrameBufferTarget target = FrameBufferTarget::Both);
        void BindRenderBuffer(FrameBufferAttachment target, unsigned renderBuffer);
        void SetDrawBuffers(const std::vector<FrameBufferAttachment>& drawBuffers);
        void SetTextureToAttachment(FrameBufferAttachment target, GlTexture& texture, int level);
        FrameBufferStatus GetStatus(FrameBufferTarget target);
        GLuint GetId() const;
    };

}


#endif //GAMEGL_GLFRAMEBUFFER_H
