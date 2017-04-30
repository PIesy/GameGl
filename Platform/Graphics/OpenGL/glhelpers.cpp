//#include "glhelpers.h"
//#include "../../../Logger/logger.h"
//#include "graphicscontextdata.h"
//#include "../../../Graphics/mesh.h"
//#include "gltexturefactory.h"
//#include <algorithm>
//
//glhelpers::VertexArrayObject glhelpers::initVAO(Mesh& mesh)
//{
//    VertexArrayObject result;
//    result.VAO = gl::GlVertexArray();
//
//    if (!mesh.GetTextures().empty())
//    {
//        Texture tex = mesh.GetTextures()[0];
//        if (!tex.info.name.empty())
//        {
//            if (globalData.Get().textureMap.count(tex.info.name))
//                result.tex = &globalData.Get().textureMap[tex.info.name];
//            else
//            {
//                if (!tex.data.empty())
//                {
//                    globalData.Get().textureMap.emplace(tex.info.name, gl::GlTextureFactory::BuildTexture(tex.info, {tex.data}));
//                    result.tex = &globalData.Get().textureMap[tex.info.name];
//                    result.tex->SetSWrapping(gl::WrappingType::ClampToBorder);
//                    result.tex->SetTWrapping(gl::WrappingType::ClampToBorder);
//                    result.tex->SetMinFilter(gl::FilterType::Linear);
//                    result.tex->SetMagFilter(gl::FilterType::Linear);
//                    result.tex->SetAnisotropicFiltering(16);
//                }
//            }
//
//        }
//    }
//
//    result.VBO = gl::GlBuffer(mesh.GetInfo().vertexCount * sizeof(Vertex), gl::BufferFlags::DynamicStorage,
//                              mesh.GetData().vertices);
//    result.IBO = gl::GlBuffer(mesh.GetInfo().indexCount * sizeof(unsigned), gl::BufferFlags::DynamicStorage,
//                              mesh.GetData().indices);
//
//    result.VAO.BindElementBuffer(result.IBO);
//    result.VAO.BindBuffer(result.VBO, 0, 0, sizeof(Vertex));
//    for (unsigned i = 0; i < 4; ++i)
//    {
//        result.VAO.EnableAttribute(i);
//        result.VAO.LinkBuffer(i, 0);
//    }
//    result.VAO.SetAttributeFormat(0, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, coords));
//    result.VAO.SetAttributeFormat(1, 4, gl::AttributeTypeFloat::Float, offsetof(Vertex, color));
//    result.VAO.SetAttributeFormat(2, 3, gl::AttributeTypeFloat::Float, offsetof(Vertex, normal));
//    result.VAO.SetAttributeFormat(3, 2, gl::AttributeTypeFloat::Float, offsetof(Vertex, uv));
//    return result;
//}
//
//void glhelpers::updateDataBuffers(Mesh& mesh, VertexArrayObject& vao)
//{
//    if (mesh.GetInfo().vertexCount * sizeof(Vertex) != vao.VBO.GetSize())
//    {
//        vao.VBO = gl::GlBuffer(mesh.GetInfo().vertexCount * sizeof(Vertex), gl::BufferFlags::DynamicStorage,
//                               mesh.GetData().vertices);
//        vao.VAO.BindBuffer(vao.VBO, 0, 0, sizeof(Vertex));
//    }
//    else
//        vao.VBO.UpdateData(mesh.GetData().vertices, mesh.GetInfo().vertexCount * sizeof(Vertex));
//
//    if (mesh.GetInfo().indexCount * sizeof(unsigned) != vao.IBO.GetSize())
//        vao.IBO = gl::GlBuffer(mesh.GetInfo().indexCount * sizeof(unsigned), gl::BufferFlags::DynamicStorage,
//                               mesh.GetData().indices);
//    else
//        vao.IBO.UpdateData(mesh.GetData().indices, mesh.GetInfo().indexCount * sizeof(unsigned));
//
//    vao.tex = nullptr;
//    if (!mesh.GetTextures().empty() && !mesh.GetTextures()[0].info.name.empty())
//        vao.tex = &globalData.Get().textureMap.at(mesh.GetTextures()[0].info.name);
//}
//
//glhelpers::FramebufferObject glhelpers::initFramebuffer(std::list<RenderTarget>& targets)
//{
//    FramebufferObject result;
//    if (std::find(targets.begin(), targets.end(), RenderTarget()) != targets.end() || targets.empty())
//        return result;
//    result.FBO = gl::GlFrameBuffer();
//
//    std::vector<gl::FrameBufferAttachment> drawBuffers(targets.size());
//    int i = 0;
//    for (RenderTarget& target : targets)
//    {
//        TextureInfo params = target.textureInfo;
//
//        gl::GlTexture* tex;
//        if (globalData.Get().textureMap.count(params.name))
//            tex = &globalData.Get().textureMap[params.name];
//        else
//        {
//            globalData.Get().textureMap.emplace(params.name, gl::GlTextureFactory::BuildTexture(params));
//            tex = &globalData.Get().textureMap[params.name];
//            tex->SetMinFilter(gl::FilterType::Nearest);
//            tex->SetMagFilter(gl::FilterType::Nearest);
//            tex->SetSWrapping(gl::WrappingType::ClampToEdge);
//            tex->SetTWrapping(gl::WrappingType::ClampToEdge);
//            tex->SetComparisonMode(gl::CompareFunction::Lequal, gl::CompareMode::ReferenceToTexture);
//        }
//
//        if (params.target != TextureBindpoint::Depth)
//        {
//            result.depthBuff = gl::GlRenderBuffer();
//            result.depthBuff.SetStorage(gl::InternalFormat::Depth32, params.width, params.height);
//            result.FBO.BindRenderBuffer(gl::FrameBufferAttachment::Depth, result.depthBuff);
//        }
//
//        if (params.target == TextureBindpoint::Color)
//        {
//            result.FBO.SetTextureToAttachment(gl::FrameBufferAttachment::Color + i, *tex, 0);
//            drawBuffers.emplace_back(gl::FrameBufferAttachment::Color + i);
//        }
//        if (params.target == TextureBindpoint::Depth)
//            result.FBO.SetTextureToAttachment(gl::FrameBufferAttachment::Depth, *tex, 0);
//    }
//    if (drawBuffers.empty())
//        drawBuffers.emplace_back(gl::FrameBufferAttachment::None);
//    result.FBO.SetDrawBuffers(drawBuffers);
//
//    if (result.FBO.GetStatus(gl::FrameBufferTarget::Both) != gl::FrameBufferStatus::Complete)
//        Logger::Log("FRAMEBUFFER INCOMPLETE!!!!AAAA!!!!");
//    gl::setViewport(targets.back().textureInfo.width, targets.back().textureInfo.height);
//    return result;
//}
