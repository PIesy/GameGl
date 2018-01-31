//
// Created by akann on 3/19/17.
//

#include "glframebuffer.h"
#include "../../../../Helpers/helpers.h"
#include "../../../../Logger/logger.h"

const auto logger = Logger::GetLogger(getClassName<gl::GlFrameBuffer>());

gl::GlFrameBuffer::GlFrameBuffer()
{
    gl::framebuffer::generate(1, &frameBuffer);
}

gl::GlFrameBuffer::GlFrameBuffer(unsigned id)
{
    this->frameBuffer = id;
}

void gl::GlFrameBuffer::Bind(gl::FrameBufferTarget target)
{
    gl::framebuffer::bind(integral(target), frameBuffer);
}

void gl::GlFrameBuffer::BindRenderBuffer(gl::FrameBufferAttachment target, GlRenderBuffer& renderBuffer)
{
    gl::framebuffer::setRenderbuffer(frameBuffer, integral(target), renderBuffer.GetId());
}

GLuint gl::GlFrameBuffer::GetId() const
{
    return frameBuffer;
}

void gl::GlFrameBuffer::SetDrawBuffers(const std::vector<gl::FrameBufferAttachment>& drawBuffers)
{
    gl::framebuffer::setDrawBuffers(frameBuffer, drawBuffers.size(), (const GLenum*)integralPointer(drawBuffers.data()));
}

gl::FrameBufferStatus gl::GlFrameBuffer::GetStatus(gl::FrameBufferTarget target)
{
    return static_cast<gl::FrameBufferStatus>(gl::framebuffer::checkStatus(frameBuffer, integral(target)));
}

void gl::GlFrameBuffer::SetTextureToAttachment(gl::FrameBufferAttachment target, gl::GlTexture& texture, int level)
{
    gl::framebuffer::setTexture(frameBuffer, integral(target), texture.GetId(), level);
}

gl::GlFrameBuffer::GlFrameBuffer(gl::GlFrameBuffer&& src) noexcept
{
    *this = std::move(src);
}

gl::GlFrameBuffer& gl::GlFrameBuffer::operator=(gl::GlFrameBuffer&& src) noexcept
{
    if (frameBuffer)
        logger.LogWarning("We have deleted valid framebuffer");
    gl::framebuffer::erase(1, &frameBuffer);
    frameBuffer = src.frameBuffer;
    src.frameBuffer = 0;
    return *this;
}

gl::GlFrameBuffer::~GlFrameBuffer()
{
    gl::framebuffer::erase(1, &frameBuffer);
}

void gl::GlFrameBuffer::Blit(gl::GlFrameBuffer& source, int srcX0, int srcY0, int srcX1, int srcY1, int targetX0, int targetY0, int targetX1, int targetY1,
                             BufferBit buffers, gl::FilterType filterType)
{
    gl::framebuffer::blit(source.GetId(), frameBuffer, srcX0, srcY0, srcX1, srcY1,
                          targetX0, targetY0, targetX1, targetY1, integral(buffers), integral(filterType));
}
