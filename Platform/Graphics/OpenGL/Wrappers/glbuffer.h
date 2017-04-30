//
// Created by akann on 3/13/17.
//

#ifndef GAMEGL_GLBUFFER_H
#define GAMEGL_GLBUFFER_H


#include <vector>
#include "glbindings.h"
#include "globject.h"
#include "../../../../Helpers/helpers.h"

namespace gl {

    enum class BufferFlags : GLbitfield
    {
        DynamicStorage = GL_DYNAMIC_STORAGE_BIT,
        MapRead = GL_MAP_READ_BIT,
        MapWrite = GL_MAP_WRITE_BIT,
        MapPersistent = GL_MAP_PERSISTENT_BIT,
        MapCoherent = GL_MAP_COHERENT_BIT,
        ClientStorage = GL_CLIENT_STORAGE_BIT
    };

    BufferFlags operator|(const BufferFlags& a, const BufferFlags& b);

    enum class BufferBindPoint : GLenum
    {
        Array = GL_ARRAY_BUFFER, ElementArray = GL_ELEMENT_ARRAY_BUFFER
    };

    class GlBuffer: public GlObject
    {
        GLuint buffer = 0;
        size_t size = 0;
        bool isElementArray = false;
        uint8_t usageHint = 0;
    public:
        GlBuffer();
        GlBuffer(size_t size, BufferFlags flags, const void* data = nullptr);
        GlBuffer(GLuint id);
        GlBuffer(const GlBuffer&) = delete;
        GlBuffer& operator =(const GlBuffer& src) = delete;
        GlBuffer(GlBuffer&& src);
        GlBuffer& operator =(GlBuffer&& src);
        ~GlBuffer();

        void UpdateData(void* data, size_t size, ptrdiff_t offset = 0);
        void Bind(BufferBindPoint bindPoint);
        GLuint GetId() const;
        size_t GetSize();
        bool IsElementArray() const;
        void SetIsElementArray(bool isElementArray);
        uint8_t GetUsageHint() const;
        void SetUsageHint(uint8_t usageHint);

    };

}


#endif //GAMEGL_GLBUFFER_H
