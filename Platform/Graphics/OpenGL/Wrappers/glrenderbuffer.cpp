//
// Created by akann on 4/15/17.
//

#include "glrenderbuffer.h"

GLuint gl::GlRenderBuffer::GetId() const
{
    return renderBuffer;
}

gl::GlRenderBuffer::GlRenderBuffer()
{
    gl::renderbuffer::generate(1, &renderBuffer);
}

gl::GlRenderBuffer::GlRenderBuffer(GLuint id)
{
    renderBuffer = id;
}

gl::GlRenderBuffer::GlRenderBuffer(gl::GlRenderBuffer&& src) noexcept
{
    *this =  std::move(src);
}

gl::GlRenderBuffer& gl::GlRenderBuffer::operator=(gl::GlRenderBuffer&& src) noexcept
{
    renderBuffer = src.renderBuffer;
    src.renderBuffer = 0;
    return *this;
}

gl::GlRenderBuffer::~GlRenderBuffer()
{
    gl::renderbuffer::erase(1, &renderBuffer);
    renderBuffer = 0;
}

void gl::GlRenderBuffer::SetStorage(gl::InternalFormat internalFormat, unsigned width, unsigned height)
{
    gl::renderbuffer::setStorage(renderBuffer, integral(internalFormat), width, height);
}
