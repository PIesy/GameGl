//
// Created by akann on 3/13/17.
//

#ifndef GAMEGL_GLBUFFER_H
#define GAMEGL_GLBUFFER_H


#include <vector>
#include "glbindings.h"
#include "globject.h"

namespace gl {

    class GlBuffer;

    class GlBufferFactory
    {
    public:
        static std::vector<GlBuffer> CreateBuffers(int count)
        {
            std::vector<GlBuffer> result(count);
            std::vector<GLuint> ids(count);

            gl::buffer::generate(count, ids.data());

            for (GLuint& id : ids)
                result.emplace_back(id);
            return result;
        }
    };

    enum class BufferUsage : GLenum
    {
        DynamicDraw = GL_DYNAMIC_DRAW, StaticDraw = GL_STATIC_DRAW, StreamDraw = GL_STREAM_DRAW,
        DynamicRead = GL_DYNAMIC_READ, StaticRead = GL_STATIC_READ, StreamRead = GL_STREAM_READ,
        DynamicCopy = GL_DYNAMIC_COPY, StaticCopy = GL_STATIC_COPY, StreamCopy = GL_STREAM_COPY
    };

    enum class BufferBindPoint : GLenum
    {
        Array = GL_ARRAY_BUFFER, ElementArray = GL_ELEMENT_ARRAY_BUFFER
    };

    class GlBuffer: public GlObject
    {
        GLuint buffer;
        size_t size = 0;
    public:
        GlBuffer();
        GlBuffer(size_t size, BufferUsage usage);
        GlBuffer(GLuint id);
        GlBuffer(GLuint id, size_t size, BufferUsage usage);
        GlBuffer(const GlBuffer&) = delete;
        GlBuffer& operator =(const GlBuffer& src) = delete;
        GlBuffer(GlBuffer&& src);
        GlBuffer& operator =(GlBuffer&& src);
        ~GlBuffer();

        void UpdateData(void* data, size_t size, ptrdiff_t offset = 0);
        void AllocateBuffer(void* data, size_t size, BufferUsage usage);
        void Bind(BufferBindPoint bindPoint);
        GLuint GetId() const;
        size_t GetSize();


    };

}


#endif //GAMEGL_GLBUFFER_H
