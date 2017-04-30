//
// Created by akann on 3/19/17.
//

#include "glframebuffer.h"
#include "../../../../Helpers/helpers.h"
#include "../../../../Logger/logger.h"

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

gl::GlFrameBuffer::GlFrameBuffer(gl::GlFrameBuffer&& src)
{
    *this = std::move(src);
}

gl::GlFrameBuffer& gl::GlFrameBuffer::operator=(gl::GlFrameBuffer&& src)
{
    if (frameBuffer)
        Logger::Log("We have deleted valid framebuffer");
    gl::framebuffer::erase(1, &frameBuffer);
    frameBuffer = src.frameBuffer;
    src.frameBuffer = 0;
    return *this;
}

gl::GlFrameBuffer::~GlFrameBuffer()
{
    gl::framebuffer::erase(1, &frameBuffer);
}
