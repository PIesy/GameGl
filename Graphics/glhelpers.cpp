#include "glhelpers.h"
#include "renderdefs.h"


glhelpers::RAIIBufferBinding::RAIIBufferBinding(GLenum target, GLuint buffer)
{
    this->target = target;
    glBindBuffer(target, buffer);
}

glhelpers::RAIIBufferBinding::~RAIIBufferBinding()
{
    glBindBuffer(target, 0);
}

glhelpers::VertexArrayObject glhelpers::initVAO(GraphicsObject& obj, bool unbindAfterInit)
{
    VertexArrayObject result;

    glGenBuffers(1, &result.VBO);
    glGenBuffers(1, &result.IBO);
    glGenVertexArrays(1, &result.VAO);
    printGlError("GEN?");

    glBindVertexArray(result.VAO);
    RAIIBufferBinding arrayBuffer(GL_ARRAY_BUFFER, result.VBO);
    RAIIBufferBinding elementBuffer(GL_ELEMENT_ARRAY_BUFFER, result.IBO);
    printGlError("BIND?");

    glBufferData(GL_ARRAY_BUFFER, obj.data().vertices.size() * sizeof(Vertex), obj.data().vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.data().indices.size() * sizeof(unsigned), obj.data().indices.data(), GL_DYNAMIC_DRAW);
    printGlError("BUFFERS?");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coords));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    if (unbindAfterInit)
    {
        glBindVertexArray(0);
    }

    return result;
}

void glhelpers::updateDataBuffers(GraphicsObject& obj, VertexArrayObject& vao)
{
    RAIIBufferBinding arrayBuffer(GL_ARRAY_BUFFER, vao.VBO);
    RAIIBufferBinding elementBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.IBO);
    printGlError("UPDATe?");

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    obj.data().vertices.size() * sizeof(Vertex),
                    obj.data().vertices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    obj.data().indices.size() * sizeof(unsigned),
                    obj.data().indices.data());
    printGlError("UPDATEV2?");
}
