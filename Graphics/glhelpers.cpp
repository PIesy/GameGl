#include "glhelpers.h"
#include "renderdefs.h"
#include "../Logger/logger.h"
#include "../Platform/OpenGL/gltexture.h"

glhelpers::VertexArrayObject glhelpers::initVAO(GraphicsObject& obj)
{
    VertexArrayObject result;

    Texture tex = obj.getTexture();
    if (obj.getTexture().data != nullptr)
        result.tex = gl::GlTexture(gl::TextureType::Tex2d);
    if (obj.getTexture().textureId != 0)
        result.externTex = true;

    if (obj.getTexture().data != nullptr)
    {
        result.tex.setSWrapping(gl::WrappingType::Repeat);
        result.tex.setTWrapping(gl::WrappingType::Repeat);
        result.tex.setMinFilter(gl::FilterType::Linear);
        result.tex.setMagFilter(gl::FilterType::Linear);
        result.tex.setSize(obj.getTexture().width, obj.getTexture().height);
        result.tex.Allocate(tex.componentsNum < 4 ? gl::InternalFormat::RGB8 : gl::InternalFormat::RGBA8);
        result.tex.Load(tex.componentsNum < 4 ? gl::TextureFormat::RGB : gl::TextureFormat::RGBA, obj.getTexture().data);
    }

    result.VBO.AllocateBuffer(obj.data().vertices.data(), obj.data().vertices.size() * sizeof(Vertex),
                              gl::BufferUsage::DynamicDraw);
    result.IBO.AllocateBuffer(obj.data().indices.data(), obj.data().indices.size() * sizeof(unsigned),
                              gl::BufferUsage::DynamicDraw);

    result.VAO.BindElementBuffer(result.IBO);
    result.VAO.BindBuffer(result.VBO, 0, 0, sizeof(Vertex));
    for (unsigned i = 0; i < 4; ++i)
    {
        result.VAO.EnableAttribute(i);
        result.VAO.LinkBuffer(i, 0);
    }
    result.VAO.SetAttributeFormat(0, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, coords));
    result.VAO.SetAttributeFormat(1, 4, gl::AttributeTypeFloat::Float, offsetof(Vertex, color));
    result.VAO.SetAttributeFormat(2, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, normal));
    result.VAO.SetAttributeFormat(3, 2, gl::AttributeTypeFloat::Float, offsetof(Vertex, uv));
    return result;
}

void glhelpers::updateDataBuffers(GraphicsObject& obj, VertexArrayObject& vao)
{
    if (obj.data().vertices.size() * sizeof(Vertex) != vao.VBO.GetSize())
        vao.VBO.AllocateBuffer(obj.data().vertices.data(), obj.data().vertices.size() * sizeof(Vertex),
                               gl::BufferUsage::DynamicDraw);
    else
        vao.VBO.UpdateData(obj.data().vertices.data(), obj.data().indices.size());

    if (obj.data().indices.size() * sizeof(unsigned) != vao.IBO.GetSize())
        vao.IBO.AllocateBuffer(obj.data().indices.data(), obj.data().indices.size() * sizeof(unsigned),
                               gl::BufferUsage::DynamicDraw);
    else
        vao.IBO.UpdateData(obj.data().indices.data(), obj.data().indices.size() * sizeof(unsigned));

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
    if (target.target == RenderTargets::SCREEN)
        return result;
    result.FBO = gl::GlFrameBuffer();
    TextureParameters params = target.textureParameters;

    gl::GlTexture tex(gl::TextureType::Tex2d);
    tex.setSize(params.width, params.height);
    if (params.type == TextureType::Color)
        tex.Allocate(gl::InternalFormat::RGBA8);
    if (params.type == TextureType::Depth)
        tex.Allocate(gl::InternalFormat::Depth32);
    tex.setMinFilter(gl::FilterType::Nearest);
    tex.setMagFilter(gl::FilterType::Nearest);
    tex.setSWrapping(gl::WrappingType::ClampToEdge);
    tex.setTWrapping(gl::WrappingType::ClampToEdge);
    tex.setComparisonMode(gl::CompareFunction::Lequal, gl::CompareMode::ReferenceToTexture);

    if (params.type != TextureType::Depth)
    {
        gl::renderbuffer::generate(1, &result.depthBuff);
        gl::renderbuffer::bind(GL_RENDERBUFFER, result.depthBuff);
        gl::renderbuffer::setStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, params.width, params.height);
        result.FBO.BindRenderBuffer(gl::FrameBufferAttachment::Depth, result.depthBuff);
    }

    if (params.type == TextureType::Color)
        result.FBO.SetTextureToAttachment(gl::FrameBufferAttachment::Color, tex, 0);
    if (params.type == TextureType::Depth)
        result.FBO.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, tex, 0);

    std::vector<gl::FrameBufferAttachment> drawBuffers(1);
    if (params.type == TextureType::Color)
        result.FBO.SetDrawBuffers({gl::FrameBufferAttachment::Color});
    if (params.type == TextureType::Depth)
        result.FBO.SetDrawBuffers({gl::FrameBufferAttachment::None});
    if (result.FBO.GetStatus(gl::FrameBufferTarget::Both) != gl::FrameBufferStatus::Complete)
        Logger::Log("FRAMEBUFFER INCOMPLETE!!!!AAAA!!!!");
    printGlError("AAAAA?");
    gl::setViewport(params.width, params.height);
    result.textures.push_back(std::move(tex));
    return result;
}
