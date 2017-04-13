//
// Created by akann on 3/13/17.
//

#include "glbuffer.h"
#include "../../Helpers/helpers.h"

namespace gl {

    GlBuffer::GlBuffer()
    {
        gl::buffer::generate(1, &buffer);
    }

    GlBuffer::GlBuffer(GLuint id)
    {
        buffer = id;
    }

    GlBuffer::GlBuffer(size_t size, BufferUsage usage)
    {
        GlBuffer();

        this->size = size;
        gl::buffer::setData(buffer, size, nullptr, integral(usage));
    }

    GlBuffer::GlBuffer(GLuint id, size_t size, BufferUsage usage)
    {
        this->size = size;
        this->buffer = id;
        gl::buffer::setData(id, size, nullptr, integral(usage));
    }

    void GlBuffer::UpdateData(void* data, size_t size, ptrdiff_t offset)
    {
        gl::buffer::updateData(buffer, offset, size, data);
    }

    void GlBuffer::AllocateBuffer(void* data, size_t size, BufferUsage usage)
    {
        this->size = size;
        gl::buffer::setData(buffer, size, data, integral(usage));
    }

    GLuint GlBuffer::GetId() const
    {
        return buffer;
    }

    size_t GlBuffer::GetSize()
    {
        return size;
    }

    void GlBuffer::Bind(BufferBindPoint bindPoint)
    {
        gl::buffer::bind(integral(bindPoint), buffer);
    }

    GlBuffer& GlBuffer::operator=(GlBuffer&& src)
    {
        size = src.size;
        buffer = src.buffer;
        src.buffer = 0;
        return *this;
    }

    GlBuffer::GlBuffer(GlBuffer&& src)
    {
        *this = std::move(src);
    }

    GlBuffer::~GlBuffer()
    {
        gl::buffer::erase(1, &buffer);
    }
}
