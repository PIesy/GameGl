//
// Created by akann on 4/29/17.
//

#include "glcontext.h"
#include "graphics_context_data.h"
#include "gltexturefactory.h"
#include "../../../Logger/logger.h"
#include "gl_program_proxy.h"
#include <algorithm>

const auto logger = Logger::GetLogger(getClassName<GlContext>());

LockedResource<gl::GlTexture> GlContext::AcquireTexture(const Texture& texture, bool read)
{
    using namespace std::string_literals;
    if (sharedGlData.Get().textureMap.Contains(texture.info.name))
        return sharedGlData.Get().textureMap.Get(texture.info.name, read);
    if (read && texture.data.empty())
    {
        static gl::GlTexture emptyTex{};
        return LocalResource<gl::GlTexture>{emptyTex};
    }

    auto result = sharedGlData.Get().textureMap.PutAndLock(texture.info.name, gl::GlTextureFactory::BuildTexture(texture.info, texture.data), read);
    logger.LogDebug("Created texture with name: "s + texture.info.name);

    configureTexture(result.GetObject(), texture.parameters, texture.info);
    if (texture.info.target == TextureBindpoint::Depth)
        result.GetObject().SetComparisonMode(gl::CompareFunction::Lequal, gl::CompareMode::ReferenceToTexture);
    return result;
}

gl::GlProgramPipeline& GlContext::GetProgramPipeline(const std::vector<std::reference_wrapper<Shader>>& programs)
{
    std::vector<std::reference_wrapper<const gl::GlProgram>> wrapper;
    size_t id = 0;

    for (const Shader& handle : programs)
    {
        const auto& glHandle = dynamic_cast<const GlProgramProxy&>(handle);
        wrapper.emplace_back(glHandle.GetProgram());
        id = (id ^ (glHandle.GetHandle() << 1u)) >> 1u;
    }
    if (data.programPipelines.count(id))
        return data.programPipelines.at(id);
    auto it = data.programPipelines.emplace(id, gl::GlProgramPipeline(wrapper));
    return it.first->second;
}

gl::GlTexture& GlContext::acquireLocalTexture(const Texture& texture)
{
    data.tempTextures.emplace_back(gl::GlTextureFactory::BuildTexture(texture.info, texture.data));
    return data.tempTextures.back();
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
    texture.SetSWrapping(wrappingType);
    texture.SetTWrapping(wrappingType);
    texture.SetRWrapping(wrappingType);

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

    if (parameters.anisotropicFiltering > 0)
        texture.SetAnisotropicFiltering(parameters.anisotropicFiltering);
}

gl::GlMeshObject& GlContext::LoadMesh(const Mesh& mesh)
{
    if (data.meshes.count(mesh.GetId()))
        return data.meshes[mesh.GetId()];

    data.meshes.emplace(mesh.GetId(), gl::GlMeshObject{});
    gl::GlMeshObject& result = data.meshes[mesh.GetId()];

    result.vertexArray = gl::GlVertexArray();
    gl::GlBuffer elementBuffer{mesh.GetInfo().indexCount * sizeof(unsigned),
                               gl::BufferFlags::DynamicStorage, mesh.GetData().indices};
    gl::GlBuffer vertexBuffer{mesh.GetInfo().vertexCount * sizeof(Vertex),
                              gl::BufferFlags::DynamicStorage, mesh.GetData().vertices};

    result.vertexArray.BindElementBuffer(elementBuffer);
    const unsigned bufferIndex = 0;
    result.vertexArray.BindBuffer(vertexBuffer, bufferIndex, 0, sizeof(Vertex));
    for (unsigned i = 0; i < 5; ++i)
    {
        result.vertexArray.EnableAttribute(i);
        result.vertexArray.LinkBuffer(i, bufferIndex);
    }
    result.vertexArray.SetAttributeFormat(0, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, coords));
    result.vertexArray.SetAttributeFormat(1, 4, gl::AttributeTypeFloat::Float, offsetof(Vertex, color));
    result.vertexArray.SetAttributeFormat(2, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, normal));
    result.vertexArray.SetAttributeFormat(3, 2, gl::AttributeTypeFloat::Float, offsetof(Vertex, uv));
    result.vertexArray.SetAttributeFormat(4, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, tangent));

    elementBuffer.SetIsElementArray(true);
    sharedGlData.Get().bufferMap.Put(mesh.GetId(), std::move(elementBuffer));
    sharedGlData.Get().bufferMap.Put(mesh.GetId(), std::move(vertexBuffer));
    
    return result;
}

gl::GlFrameBufferContainer& GlContext::BuildFrameBuffer(const RenderStep& step, const std::vector<RenderStep>& steps,
                                                        unsigned viewportWidth, unsigned viewportHeight)
{
    const std::list<RenderTarget>& targets = step.targets;

    if (step.frameBufferProperties.frameBufferType == FrameBufferProperties::REUSE_FROM_STEP)
    {
        data.frameBuffers.emplace_back(gl::GlFrameBufferContainer{});
        gl::GlFrameBufferContainer* result = &data.frameBuffers[step.frameBufferProperties.stepId];
        if (result->reference != nullptr)
            result = result->reference;
        data.frameBuffers.back().reference = result;
        if (result->frameBuffer.GetId() == 0)
            gl::setViewport(viewportWidth, viewportHeight);
        else
        {
            TextureInfo info = steps[step.frameBufferProperties.stepId].targets.begin()->texture.info;
            gl::setViewport(info.width, info.height);
        }
        return *result;
    }

    if (targets.empty() || targets.begin()->target == RenderTarget::SCREEN)
    {
        data.frameBuffers.emplace_back(gl::GlFrameBufferContainer{});
        if (step.frameBufferProperties.frameBufferType == FrameBufferProperties::NEW_COPY_BUFFERS_FROM)
            blitFrameBuffers(data.frameBuffers[step.frameBufferProperties.stepId], data.frameBuffers.back(), step.frameBufferProperties, viewportWidth, viewportHeight);
        gl::setViewport(viewportWidth, viewportHeight);
        return data.frameBuffers.back();
    }

    data.frameBuffers.push_back({gl::GlFrameBuffer{}, std::vector<gl::GlRenderBuffer>{}});
    gl::GlFrameBufferContainer& result = data.frameBuffers.back();
    TextureInfo info = targets.begin()->texture.info;
    TextureParameters parameters = targets.begin()->texture.parameters;
    int i = 0;
    bool depthInitialized = false;
    bool stencilInitialized = false;
    std::vector<gl::FrameBufferAttachment> drawBuffers;

    for (const RenderTarget& target : targets)
    {
        data.lockedTextures.push_back(AcquireTexture(target.texture, false));
        switch (target.texture.info.target)
        {
            case TextureBindpoint::Color:
                drawBuffers.push_back(gl::FrameBufferAttachment::Color + i);
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Color + i, data.lockedTextures.back().GetObject(), target.texture.parameters.useMipLevel);
                break;
            case TextureBindpoint::Depth:
                depthInitialized = true;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, data.lockedTextures.back().GetObject(), target.texture.parameters.useMipLevel);
                break;
            case TextureBindpoint::Stencil:
                stencilInitialized = true;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Stencil, data.lockedTextures.back().GetObject(), target.texture.parameters.useMipLevel);
                break;
            case TextureBindpoint::DepthStencil:
                depthInitialized = true;
                stencilInitialized = true;
                result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::DepthStencil, data.lockedTextures.back().GetObject(), target.texture.parameters.useMipLevel);
        }
    }

    if (!depthInitialized && state.depthTest)
    {
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
            depth.info.width = depth.info.width / (1u << depth.parameters.useMipLevel);
            depth.info.height = depth.info.height / (1u << depth.parameters.useMipLevel);
            depth.info.target = TextureBindpoint::Depth;
            depth.info.mipmaps = 0;
            depth.info.targetPixelFormat = TexturePixelFormat::Float24;
            result.frameBuffer.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, acquireLocalTexture(depth), 0);
        }
    }

    if (!stencilInitialized && state.stencilTest)
    {
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
        logger.LogError("Could not create framebuffer");

    gl::setViewport(info.width / (1u << parameters.useMipLevel), info.height / (1u << parameters.useMipLevel));
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

    // Wireframe
    if (std::find(attributes.begin(), attributes.end(), RenderingAttribute::WireframeMode) != attributes.end())
    {
        if (!state.wireframe)
        {
            state.wireframe = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    else
    {
        if (state.wireframe)
        {
            state.wireframe = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
    data.frameBuffers.clear();
    data.frameBuffers.shrink_to_fit();
    data.tempTextures.clear();
    data.tempTextures.shrink_to_fit();
}

void GlContext::blitFrameBuffers(gl::GlFrameBufferContainer& src, gl::GlFrameBufferContainer& target, const FrameBufferProperties& properties,
                                 unsigned width, unsigned height)
{
    if (properties.sourceBuffer == FrameBufferProperties::ALL || properties.sourceBuffer & FrameBufferProperties::COLOR)
        target.frameBuffer.Blit(src.frameBuffer, 0, 0, width, height, 0, 0, width, height, gl::BufferBit::Color, gl::FilterType::Linear);

    gl::BufferBit mask = gl::BufferBit::None;
    if (properties.sourceBuffer == FrameBufferProperties::ALL)
        mask |= gl::BufferBit::Stencil | gl::BufferBit::Depth;
    else
    {
        if (properties.sourceBuffer & FrameBufferProperties::DEPTH)
            mask |= gl::BufferBit::Depth;
        if (properties.sourceBuffer & FrameBufferProperties::STENCIL)
            mask |= gl::BufferBit::Stencil;
    }
    target.frameBuffer.Blit(src.frameBuffer, 0, 0, width, height, 0, 0, width, height, mask, gl::FilterType::Nearest);
}

void GlContext::UpdateFrameState(gl::GlFrameBufferContainer& frameBuffer, const RenderingAttributes& attributes)
{
    if (!data.lockedTextures.empty())
        gl::flush();
    if (!data.lockedTextures.empty() && std::find(attributes.begin(), attributes.end(), RenderingAttribute::UpdateMipmaps) != attributes.end())
    {
        for (auto& tex : data.lockedTextures)
            gl::texture::generateMipmaps(tex.GetObject().GetId());
    }
    data.lockedTextures.clear();
    data.lockedTextures.shrink_to_fit();
}

