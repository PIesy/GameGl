//
// Created by akann on 4/29/17.
//

#include "glcontext.h"
#include "graphicscontextdata.h"
#include "gltexturefactory.h"
#include "../../../Logger/logger.h"
#include <algorithm>

gl::GlTexture& GlContext::LoadTexture(const Texture& texture)
{
    if (texture.info.name.empty())
        return emptyTexture;
    if (globalData.Get().textureMap.count(texture.info.name))
        return globalData.Get().textureMap[texture.info.name];

    globalData.Get().textureMap.emplace(texture.info.name, gl::GlTextureFactory::BuildTexture(texture.info, texture.data));
    gl::GlTexture& result = globalData.Get().textureMap[texture.info.name];

    configureTexture(result, texture.parameters, texture.info);
    if (texture.info.target == TextureBindpoint::Depth)
        result.SetComparisonMode(gl::CompareFunction::Lequal, gl::CompareMode::ReferenceToTexture);
    return result;
}

void GlContext::configureTexture(gl::GlTexture& texture, const TextureParameters& parameters, const TextureInfo& info)
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
            texture.SetMinFilter((info.mipmaps == 0 ? gl::FilterType::Linear : gl::FilterType::MipmapLinear));
            texture.SetMagFilter(gl::FilterType::Linear);
            break;
        case TextureSampling::Nearest:
            texture.SetMinFilter((info.mipmaps == 0 ? gl::FilterType::Nearest : gl::FilterType::MipmapNearest));
            texture.SetMagFilter(gl::FilterType::Nearest);
    }

    texture.SetSWrapping(wrappingType);
    texture.SetTWrapping(wrappingType);
    texture.SetRWrapping(wrappingType);

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
    for (unsigned i = 0; i < 5; ++i)
    {
        result.vertexArray.EnableAttribute(i);
        result.vertexArray.LinkBuffer(i, 0);
    }
    result.vertexArray.SetAttributeFormat(0, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, coords));
    result.vertexArray.SetAttributeFormat(1, 4, gl::AttributeTypeFloat::Float, offsetof(Vertex, color));
    result.vertexArray.SetAttributeFormat(2, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, normal));
    result.vertexArray.SetAttributeFormat(3, 2, gl::AttributeTypeFloat::Float, offsetof(Vertex, uv));
    result.vertexArray.SetAttributeFormat(4, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, tangent));

    elementBuffer.SetIsElementArray(true);
    globalData.Get().bufferMap.emplace(mesh.GetId(), std::move(elementBuffer));
    globalData.Get().bufferMap.emplace(mesh.GetId(), std::move(vertexBuffer));
    
    return result;
}

gl::GlFrameBufferContainer& GlContext::BuildFrameBuffer(const RenderStep& step, const std::vector<RenderStep>& steps,
                                                        unsigned viewportWidth, unsigned viewportHeight)
{
    const std::list<RenderTarget>& targets = step.targets;

    if (step.frameBufferProperties.frameBufferType == FrameBufferProperties::REUSE_FROM_STEP)
    {
        frameBuffers.emplace_back(gl::GlFrameBufferContainer{});
        gl::GlFrameBufferContainer& result = frameBuffers[step.frameBufferProperties.stepId];
        if (result.frameBuffer.GetId() == 0)
            gl::setViewport(viewportWidth, viewportHeight);
        else
        {
            TextureInfo info = steps[step.frameBufferProperties.stepId].targets.begin()->texture.info;
            gl::setViewport(info.width, info.height);
        }
        return result;
    }

    if (targets.empty() || targets.begin()->target == RenderTarget::SCREEN)
    {
        frameBuffers.emplace_back(gl::GlFrameBufferContainer{});
        if (step.frameBufferProperties.frameBufferType == FrameBufferProperties::NEW_COPY_BUFFERS_FROM)
            blitFrameBuffers(frameBuffers[step.frameBufferProperties.stepId], frameBuffers.back(), step.frameBufferProperties, viewportWidth, viewportHeight);
        gl::setViewport(viewportWidth, viewportHeight);
        return frameBuffers.back();
    }

    frameBuffers.push_back({gl::GlFrameBuffer{}, std::vector<gl::GlRenderBuffer>{}});
    gl::GlFrameBufferContainer& result = frameBuffers.back();
    TextureInfo info = targets.begin()->texture.info;
    TextureParameters parameters = targets.begin()->texture.parameters;
    GLbitfield clearMask = 0;
    int i = 0;
    bool depthInitialized = false;
    bool stencilInitialized = false;
    std::vector<gl::FrameBufferAttachment> drawBuffers;

    for (const RenderTarget& target : targets)
    {
        result.textures.push_back(std::ref(LoadTexture(target.texture)));
        switch (target.texture.info.target)
        {
            case TextureBindpoint::Color:
                clearMask |= GL_COLOR_BUFFER_BIT;
                drawBuffers.push_back(gl::FrameBufferAttachment::Color + i);
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Color + i, LoadTexture(target.texture), target.texture.parameters.useMipLevel);
                break;
            case TextureBindpoint::Depth:
                depthInitialized = true;
                clearMask |= GL_DEPTH_BUFFER_BIT;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, LoadTexture(target.texture), target.texture.parameters.useMipLevel);
                break;
            case TextureBindpoint::Stencil:
                clearMask |= GL_STENCIL_BUFFER_BIT;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Stencil, LoadTexture(target.texture), target.texture.parameters.useMipLevel);
                break;
            case TextureBindpoint::DepthStencil:
                depthInitialized = true;
                stencilInitialized = true;
                clearMask |= GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::DepthStencil, LoadTexture(target.texture), target.texture.parameters.useMipLevel);
        }
    }

    if (!depthInitialized && state.depthTest)
    {
        clearMask &= GL_DEPTH_BUFFER_BIT;
        if (info.count == 1)
        {
            result.renderBuffers.emplace_back(gl::GlRenderBuffer{});
            gl::GlRenderBuffer& depthBuffer = result.renderBuffers.back();
            depthBuffer.SetStorage(gl::InternalFormat::Depth32, info.width, info.height);
            result.frameBuffer.BindRenderBuffer(gl::FrameBufferAttachment::Depth, depthBuffer);
        }
        else
        {
            Texture depth = targets.front().texture;
            depth.info.width = depth.info.width / (int)std::pow(2, depth.parameters.useMipLevel);
            depth.info.height = depth.info.height / (int)std::pow(2, depth.parameters.useMipLevel);
            depth.info.target = TextureBindpoint::Depth;
            depth.info.mipmaps = 0;
            depth.info.name = "layeredDepth" + std::to_string(usedDepthTextures++);
            depth.info.targetPixelFormat = TexturePixelFormat::Float24;
            result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, LoadTexture(depth), 0);
        }
    }

    if (!stencilInitialized && state.stencilTest)
    {
        clearMask &= GL_STENCIL_BUFFER_BIT;
        result.renderBuffers.emplace_back(gl::GlRenderBuffer{});
        gl::GlRenderBuffer& stencilBuffer = result.renderBuffers.back();
        stencilBuffer.SetStorage(gl::InternalFormat::Stencil, info.width, info.height);
        result.frameBuffer.BindRenderBuffer(gl::FrameBufferAttachment::Stencil, stencilBuffer);
    }

    if (drawBuffers.empty())
        result.frameBuffer.SetDrawBuffers({gl::FrameBufferAttachment::None});
    else
        result.frameBuffer.SetDrawBuffers(drawBuffers);

    if (result.frameBuffer.GetStatus(gl::FrameBufferTarget::Both) != gl::FrameBufferStatus::Complete)
        Logger::Log("Could not create framebuffer");

    gl::setViewport(info.width / (int)std::pow(2, parameters.useMipLevel), info.height / (int)std::pow(2, parameters.useMipLevel));
    if (clearMask)
        gl::clear(clearMask);
    return result;
}

void GlContext::UpdateState(const RenderingAttributes& attributes)
{
    // Depth test
    if (std::find(attributes.begin(), attributes.end(), RenderingAttribute::DepthTest) != attributes.end())
    {
        if (!state.depthTest)
        {
            state.depthTest = true;
            gl::enable(GL_DEPTH_TEST);
        }
    }
    else
    {
        if (state.depthTest)
        {
            state.depthTest = false;
            gl::disable(GL_DEPTH_TEST);
        }
    }


    // Blend
    if (std::find(attributes.begin(), attributes.end(), RenderingAttribute::Blend) != attributes.end())
    {
        if (!state.blend)
        {
            state.blend = true;
            gl::enable(GL_BLEND);
        }
    }
    else
    {
        if (state.blend)
        {
            state.blend = false;
            gl::disable(GL_BLEND);
        }
    }

    // Stencil
    if (std::find(attributes.begin(), attributes.end(), RenderingAttribute::StencilTest) != attributes.end())
    {
        if (!state.stencilTest)
        {
            state.stencilTest = true;
            gl::enable(GL_STENCIL_TEST);
        }
    }
    else
    {
        if (state.stencilTest)
        {
            state.stencilTest = false;
            gl::disable(GL_STENCIL_TEST);
        }
    }

    // Culling
    bool cullBack;

    if ((cullBack = std::find(attributes.begin(), attributes.end(), RenderingAttribute::CullBack) != attributes.end())
        || std::find(attributes.begin(), attributes.end(), RenderingAttribute::CullFace) != attributes.end())
    {
        if (!state.culling)
        {
            state.culling = true;
            gl::enable(GL_CULL_FACE);
            if (cullBack)
            {
                if (state.cullFace)
                {
                    state.cullFace = false;
                    gl::culling::cullFace(GL_BACK);
                }
            }
            else
            {
                if (!state.cullFace)
                {
                    state.cullFace = true;
                    gl::culling::cullFace(GL_FRONT);
                }
            }
        }
    }
    else
    {
        if (state.culling)
        {
            state.culling = false;
            gl::disable(GL_CULL_FACE);
        }
    }
}

void GlContext::UpdateFrameBuffer(const RenderingAttributes& attributes)
{
    if (std::find(attributes.begin(), attributes.end(), RenderingAttribute::ClearBuffer) != attributes.end())
    {
        GLbitfield clearFlags = GL_COLOR_BUFFER_BIT;

        if (state.depthTest)
            clearFlags |= GL_DEPTH_BUFFER_BIT;
        if (state.stencilTest)
            clearFlags |= GL_STENCIL_BUFFER;
        gl::clear(clearFlags);
    }
}

void GlContext::ClearFrameState()
{
    frameBuffers.clear();
    frameBuffers.shrink_to_fit();
    usedDepthTextures = 0;
}

void GlContext::blitFrameBuffers(gl::GlFrameBufferContainer& src, gl::GlFrameBufferContainer& target, const FrameBufferProperties& properties,
                                 unsigned width, unsigned height)
{
    if (properties.sourceBuffer == FrameBufferProperties::ALL || properties.sourceBuffer & FrameBufferProperties::COLOR)
        gl::framebuffer::blit(src.frameBuffer.GetId(), target.frameBuffer.GetId(), 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    GLbitfield mask = 0;
    if (properties.sourceBuffer == FrameBufferProperties::ALL)
        mask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
    else
    {
        if (properties.sourceBuffer & FrameBufferProperties::DEPTH)
            mask |= GL_DEPTH_BUFFER_BIT;
        if (properties.sourceBuffer & FrameBufferProperties::STENCIL)
            mask |= GL_STENCIL_BUFFER_BIT;
    }
    gl::framebuffer::blit(src.frameBuffer.GetId(), target.frameBuffer.GetId(), 0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);
}

void GlContext::UpdateFrameState(gl::GlFrameBufferContainer& frameBuffer, const RenderingAttributes& attributes)
{
    if (frameBuffer.textures.size() && std::find(attributes.begin(), attributes.end(), RenderingAttribute::UpdateMipmaps) != attributes.end())
    {
        for (gl::GlTexture& tex : frameBuffer.textures)
            gl::texture::generateMipmaps(tex.GetId());
    }

}
