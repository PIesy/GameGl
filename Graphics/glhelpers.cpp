#include "glhelpers.h"
#include "renderdefs.h"
#include "Logger/logger.h"

glhelpers::RAIIBufferBinding::RAIIBufferBinding(GLenum target, GLuint buffer)
{
    this->target = target;
    gl::buffer::bind(target, buffer);
}

glhelpers::RAIIBufferBinding::~RAIIBufferBinding()
{
    gl::buffer::bind(target, 0);
}

glhelpers::VertexArrayObject glhelpers::initVAO(GraphicsObject& obj, bool unbindAfterInit)
{
    VertexArrayObject result;

    if (obj.getTexture().data != nullptr)
        gl::texture::generate(1, &result.tex);
    if (obj.getTexture().textureId != 0)
        result.externTex = true;
    gl::buffer::generate(1, &result.VBO);
    gl::buffer::generate(1, &result.IBO);
    gl::vertexarray::generate(1, &result.VAO);

    gl::vertexarray::bind(result.VAO);
    if (obj.getTexture().data != nullptr)
        gl::texture::bind(GL_TEXTURE_2D, result.tex);
    RAIIBufferBinding arrayBuffer(GL_ARRAY_BUFFER, result.VBO);
    RAIIBufferBinding elementBuffer(GL_ELEMENT_ARRAY_BUFFER, result.IBO);

    if (obj.getTexture().data != nullptr)
    {
        float anisotropySamples;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropySamples);
        gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropySamples);
        gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gl::texture::load2Dimage(GL_TEXTURE_2D, 0, GL_RGB, obj.getTexture().width,
                     obj.getTexture().height, GL_RGB, GL_UNSIGNED_BYTE, obj.getTexture().data);
        gl::texture::bind(GL_TEXTURE_2D, 0);
    }

    gl::buffer::setData(GL_ARRAY_BUFFER, obj.data().vertices.size() * sizeof(Vertex), obj.data().vertices.data(), GL_DYNAMIC_DRAW);
    gl::buffer::setData(GL_ELEMENT_ARRAY_BUFFER, obj.data().indices.size() * sizeof(unsigned), obj.data().indices.data(), GL_DYNAMIC_DRAW);

    gl::vertexarray::enableVertexAttribute(0);
    gl::vertexarray::enableVertexAttribute(1);
    gl::vertexarray::enableVertexAttribute(2);
    gl::vertexarray::enableVertexAttribute(3);
    gl::vertexarray::setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coords));
    gl::vertexarray::setVertexAttributePointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    gl::vertexarray::setVertexAttributePointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    gl::vertexarray::setVertexAttributePointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    if (unbindAfterInit)
    {
        gl::vertexarray::bind(0);
    }

    return result;
}

void glhelpers::updateDataBuffers(GraphicsObject& obj, VertexArrayObject& vao)
{
    RAIIBufferBinding arrayBuffer(GL_ARRAY_BUFFER, vao.VBO);
    RAIIBufferBinding elementBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.IBO);

    gl::buffer::updateData(GL_ARRAY_BUFFER, 0,
                    obj.data().vertices.size() * sizeof(Vertex),
                    obj.data().vertices.data());
    gl::buffer::updateData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    obj.data().indices.size() * sizeof(unsigned),
                    obj.data().indices.data());
    if (obj.getTexture().textureId != 0)
    {
        vao.externTex = true;
        vao.stepId = obj.getTexture().stepId;
        vao.texId = obj.getTexture().textureId;
    }
    if (obj.getTexture().data != nullptr && obj.getTexture().textureId == 0)
        vao.externTex = false;
}

glhelpers::FramebufferObject glhelpers::initFramebuffer(const RenderTarget& target)
{
    FramebufferObject result;
    if (target.target == UsableRenderTargets::SCREEN)
        return result;
    TextureParameters params = target.textureParameters;

    gl::framebuffer::generate(1, &result.FBO);
    gl::framebuffer::bind(GL_FRAMEBUFFER, result.FBO);

    GLuint tex;
    gl::texture::generate(1, &tex);
    result.textures.push_back(tex);
    gl::texture::bind(GL_TEXTURE_2D, tex);
    if (params.type == TextureType::Color)
        gl::texture::load2Dimage(GL_TEXTURE_2D, 0, GL_RGBA, params.width, params.height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    if (params.type == TextureType::Depth)
        gl::texture::load2Dimage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, params.width, params.height, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    gl::texture::setParameter(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    if (params.type != TextureType::Depth)
    {
        gl::renderbuffer::generate(1, &result.depthBuff);
        gl::renderbuffer::bind(GL_RENDERBUFFER, result.depthBuff);
        gl::renderbuffer::setStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, params.width, params.height);
        gl::framebuffer::setRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, result.depthBuff);
    }

    if (params.type == TextureType::Color)
        gl::framebuffer::set2Dtexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    if (params.type == TextureType::Depth)
        gl::framebuffer::set2Dtexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);

    GLenum drawBuffers[1];
    if (params.type == TextureType::Color)
        drawBuffers[0] = {GL_COLOR_ATTACHMENT0};
    if (params.type == TextureType::Depth)
        drawBuffers[0] = {GL_NONE};
    glDrawBuffers(1, drawBuffers);
    printGlError("DRAW BUFFERS?");
    printGlError("AAAAA?");
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::Log("FRAMEBUFFER INCOMPLETE!!!!AAAA!!!!");
    gl::setViewport(params.width, params.height);
    return result;
}
