//
// Created by akann on 3/26/18.
//

#include "glfence.h"

gl::GlFence::GlFence()
{
    fence = gl::sync::createFence();
}

gl::GlFence::~GlFence()
{
    gl::sync::erase(fence);
}

gl::GlFence& gl::GlFence::operator=(gl::GlFence&& src) noexcept
{
    gl::sync::erase(fence);
    fence = 0;
    std::swap(fence, src.fence);
    return *this;
}

gl::GlFence::GlFence(gl::GlFence&& src) noexcept
{
    *this = std::move(src);
}

void gl::GlFence::Wait()
{
    gl::sync::wait(fence, 0, GL_TIMEOUT_IGNORED);
}

void gl::GlFence::ClientWait()
{

    gl::sync::wait(fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
}
