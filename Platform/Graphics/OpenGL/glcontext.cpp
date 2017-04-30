//
// Created by akann on 4/29/17.
//

#include "glcontext.h"
#include "graphicscontextdata.h"
#include "gltexturefactory.h"
#include "glframebuffercontainer.h"
#include "../../../Logger/logger.h"


gl::GlTexture& GlContext::LoadTexture(const Texture& texture)
{
    if (texture.info.name.empty())
        return emptyTexture;
    if (globalData.Get().textureMap.count(texture.info.name))
        return globalData.Get().textureMap[texture.info.name];

    globalData.Get().textureMap.emplace(texture.info.name, gl::GlTextureFactory::BuildTexture(texture.info, texture.data));
    gl::GlTexture& result = globalData.Get().textureMap[texture.info.name];

    configureTexture(result, texture.parameters);
    if (texture.info.target == TextureBindpoint::Depth)
        result.SetComparisonMode(gl::CompareFunction::Lequal, gl::CompareMode::ReferenceToTexture);
    return result;
}

void GlContext::configureTexture(gl::GlTexture& texture, const TextureParameters& parameters)
{
    gl::WrappingType wrappingType;
    switch (parameters.wrapping)
    {
        case TextureWrapping::Clamp:
            wrappingType = gl::WrappingType::ClampToEdge;
            break;
        case TextureWrapping::None:
            wrappingType = gl::WrappingType::ClampToBorder;
            break;
        case TextureWrapping::Repeat:
            wrappingType = gl::WrappingType::Repeat;
            break;
        case TextureWrapping::MirroredRepeat:
            wrappingType = gl::WrappingType::MirroredRepeat;
    }

    gl::FilterType filterType;
    switch (parameters.sampling)
    {

        case TextureSampling::Linear:
            filterType = gl::FilterType::Linear;
            break;
        case TextureSampling::Nearest:
            filterType = gl::FilterType::Nearest;
    }

    texture.SetSWrapping(wrappingType);
    texture.SetTWrapping(wrappingType);
    texture.SetMinFilter(filterType);
    texture.SetMagFilter(filterType);

    if (parameters.anisotropicFiltering > 0)
        texture.SetAnisotropicFiltering(parameters.anisotropicFiltering);
}

gl::GlMeshObject& GlContext::LoadMesh(const Mesh& mesh)
{
    if (meshes.count(mesh.GetId()))
        return meshes[mesh.GetId()];

    meshes.emplace(mesh.GetId(), gl::GlMeshObject{});
    gl::GlMeshObject& result = meshes[mesh.GetId()];

    for (const Texture& texture : mesh.GetTextures())
        result.textures.emplace_back(LoadTexture(texture));

    result.vertexArray = gl::GlVertexArray();
    gl::GlBuffer elementBuffer{mesh.GetInfo().indexCount * sizeof(unsigned),
                               gl::BufferFlags::DynamicStorage, mesh.GetData().indices};
    gl::GlBuffer vertexBuffer{mesh.GetInfo().vertexCount * sizeof(Vertex),
                              gl::BufferFlags::DynamicStorage, mesh.GetData().vertices};

    result.vertexArray.BindElementBuffer(elementBuffer);
    result.vertexArray.BindBuffer(vertexBuffer, 0, 0, sizeof(Vertex));
    for (unsigned i = 0; i < 4; ++i)
    {
        result.vertexArray.EnableAttribute(i);
        result.vertexArray.LinkBuffer(i, 0);
    }
    result.vertexArray.SetAttributeFormat(0, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, coords));
    result.vertexArray.SetAttributeFormat(1, 4, gl::AttributeTypeFloat::Float, offsetof(Vertex, color));
    result.vertexArray.SetAttributeFormat(2, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, normal));
    result.vertexArray.SetAttributeFormat(3, 2, gl::AttributeTypeFloat::Float, offsetof(Vertex, uv));

    elementBuffer.SetIsElementArray(true);
    globalData.Get().bufferMap.emplace(mesh.GetId(), std::move(elementBuffer));
    globalData.Get().bufferMap.emplace(mesh.GetId(), std::move(vertexBuffer));
    
    return result;
}

gl::GlFrameBufferContainer GlContext::BuildFrameBuffer(const std::list<RenderTarget>& targets)
{
    if (targets.empty() || targets.begin()->target == RenderTarget::SCREEN)
        return gl::GlFrameBufferContainer{};

    gl::GlFrameBufferContainer result;
    result.frameBuffer = gl::GlFrameBuffer{};

    int i = 0;
    bool depthInitialized = false;

    std::vector<gl::FrameBufferAttachment> drawBuffers;
    for (const RenderTarget& target : targets)
    {
        switch (target.texture.info.target)
        {
            case TextureBindpoint::Color:
                drawBuffers.push_back(gl::FrameBufferAttachment::Color + i);
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Color + i, LoadTexture(target.texture), 0);
                break;
            case TextureBindpoint::Depth:
                depthInitialized = true;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, LoadTexture(target.texture), 0);
                break;
            case TextureBindpoint::Stencil:
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Stencil, LoadTexture(target.texture), 0);
                break;
            case TextureBindpoint::DepthStencil:
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::DepthStencil, LoadTexture(target.texture), 0);
        }
    }

    TextureInfo info = targets.begin()->texture.info;
    if (!depthInitialized)
    {
        result.renderBuffers.emplace_back(gl::GlRenderBuffer{});
        gl::GlRenderBuffer& depthBuffer = result.renderBuffers.back();
        depthBuffer.SetStorage(gl::InternalFormat::Depth32, info.width, info.height);
        result.frameBuffer.BindRenderBuffer(gl::FrameBufferAttachment::Depth, depthBuffer);
    }
    if (drawBuffers.empty())
        result.frameBuffer.SetDrawBuffers({gl::FrameBufferAttachment::None});
    else
        result.frameBuffer.SetDrawBuffers(drawBuffers);
    if (result.frameBuffer.GetStatus(gl::FrameBufferTarget::Both) != gl::FrameBufferStatus::Complete)
        Logger::Log("Something is very wrong");
    gl::setViewport(info.width, info.height);
    return result;
}
