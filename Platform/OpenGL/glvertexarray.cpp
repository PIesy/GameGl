//
// Created by akann on 3/16/17.
//

#include "glvertexarray.h"

namespace gl {

    GlVertexArray::GlVertexArray()
    {
        gl::vertexarray::generate(1, &vertexArray);
    }

    GlVertexArray::GlVertexArray(GLuint id)
    {
        this->vertexArray = id;
    }

    void GlVertexArray::BindElementBuffer(gl::GlBuffer& elementBuffer)
    {
        gl::vertexarray::bindElementBuffer(vertexArray, elementBuffer.GetId());
    }

    void GlVertexArray::BindBuffer(gl::GlBuffer& buffer, unsigned index, ptrdiff_t offset, int size)
    {
        gl::vertexarray::bindVertexBuffer(vertexArray, index, buffer.GetId(), offset, size);
    }

    void GlVertexArray::EnableAttribute(unsigned index)
    {
        gl::vertexarray::enableVertexAttribute(vertexArray, index);
    }

    void GlVertexArray::DisableAttribute(unsigned index)
    {
        gl::vertexarray::disableVertexAttribute(vertexArray, index);
    }

    void GlVertexArray::SetAttributeFormat(unsigned index, int size, AttributeTypeFloat type, unsigned offset,
                                           bool normalized)
    {
        gl::vertexarray::setVertexAttributeFormat(vertexArray, index, size, integral(type), (unsigned char) normalized,
                                                  offset);
    }

    void GlVertexArray::LinkBuffer(unsigned attributeIndex, unsigned bufferIndex)
    {
        gl::vertexarray::bindBufferToAttribute(vertexArray, attributeIndex, bufferIndex);
    }

    void GlVertexArray::Bind()
    {
        gl::vertexarray::bind(vertexArray);
    }

    GlVertexArray::GlVertexArray(GlVertexArray&& src)
    {
        *this = std::move(src);
    }

    GlVertexArray::~GlVertexArray()
    {
        gl::vertexarray::erase(1, &vertexArray);
    }

    GlVertexArray& GlVertexArray::operator=(GlVertexArray&& src)
    {
        vertexArray = src.vertexArray;
        src.vertexArray = 0;
        return *this;
    }
}
