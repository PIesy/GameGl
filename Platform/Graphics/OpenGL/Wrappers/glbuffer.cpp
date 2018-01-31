//
// Created by akann on 3/13/17.
//

#include "glbuffer.h"
#include "../../../../Logger/logger.h"

namespace gl {

    GlBuffer::GlBuffer(GLuint id)
    {
        buffer = id;
    }

    GlBuffer::GlBuffer(size_t size, BufferFlags flags, const void* data)
    {
        gl::buffer::generate(1, &buffer);

        this->size = size;
        gl::buffer::setStorage(buffer, size, data, integral(flags));
    }

    void GlBuffer::UpdateData(void* data, size_t size, ptrdiff_t offset)
    {
        gl::buffer::updateData(buffer, offset, size, data);
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

    GlBuffer& GlBuffer::operator=(GlBuffer&& src) noexcept
    {
        gl::buffer::erase(1, &buffer);
        size = src.size;
        buffer = src.buffer;
        isElementArray = src.isElementArray;
        src.buffer = 0;
        return *this;
    }

    GlBuffer::GlBuffer(GlBuffer&& src) noexcept
    {
        *this = std::move(src);
    }

    GlBuffer::~GlBuffer()
    {
        gl::buffer::erase(1, &buffer);
    }

    GlBuffer::GlBuffer()
    {

    }

    bool GlBuffer::IsElementArray() const
    {
        return isElementArray;
    }

    void GlBuffer::SetIsElementArray(bool isElementArray)
    {
        GlBuffer::isElementArray = isElementArray;
    }

    BufferFlags operator|(const BufferFlags& a, const BufferFlags& b)
    {
        return static_cast<BufferFlags>(integral(a) | integral(b));
    }
}
