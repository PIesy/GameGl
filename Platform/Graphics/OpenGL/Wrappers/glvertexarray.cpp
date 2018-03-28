//
// Created by akann on 3/16/17.
//

#include "glvertexarray.h"

namespace gl {

    GlVertexArray::GlVertexArray()
    {
        gl::vertex_array::generate(1, &vertexArray);
    }

    GlVertexArray::GlVertexArray(GLuint id)
    {
        this->vertexArray = id;
    }

    void GlVertexArray::BindElementBuffer(gl::GlBuffer& elementBuffer)
    {
        gl::vertex_array::bindElementBuffer(vertexArray, elementBuffer.GetId());
    }

    void GlVertexArray::BindBuffer(gl::GlBuffer& buffer, unsigned index, ptrdiff_t offset, int size)
    {
        gl::vertex_array::bindVertexBuffer(vertexArray, index, buffer.GetId(), offset, size);
    }

    void GlVertexArray::UnbindBuffer(unsigned index)
    {
        gl::vertex_array::bindVertexBuffer(vertexArray, index, 0, 0, 0);
    }

    void GlVertexArray::EnableAttribute(unsigned index)
    {
        gl::vertex_array::enableVertexAttribute(vertexArray, index);
    }

    void GlVertexArray::DisableAttribute(unsigned index)
    {
        gl::vertex_array::disableVertexAttribute(vertexArray, index);
    }

    void GlVertexArray::SetAttributeFormat(unsigned index, int size, AttributeTypeFloat type, unsigned offset,
                                           bool normalized)
    {
        gl::vertex_array::setVertexAttributeFormat(vertexArray, index, size, integral(type), (unsigned char) normalized,
                                                  offset);
    }

    void GlVertexArray::LinkBuffer(unsigned attributeIndex, unsigned bufferIndex)
    {
        gl::vertex_array::bindBufferToAttribute(vertexArray, attributeIndex, bufferIndex);
    }

    void GlVertexArray::Bind()
    {
        gl::vertex_array::bind(vertexArray);
    }

    GlVertexArray::GlVertexArray(GlVertexArray&& src) noexcept
    {
        *this = std::move(src);
    }

    GlVertexArray::~GlVertexArray()
    {
        gl::vertex_array::erase(1, &vertexArray);
    }

    GlVertexArray& GlVertexArray::operator=(GlVertexArray&& src) noexcept
    {
        gl::vertex_array::erase(1, &vertexArray);
        vertexArray = src.vertexArray;
        src.vertexArray = 0;
        return *this;
    }
}
