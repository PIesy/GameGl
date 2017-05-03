//
// Created by akann on 4/29/17.
//

#ifndef GAMEGL_GLCONTEXT_H
#define GAMEGL_GLCONTEXT_H

#include <unordered_map>
#include "Wrappers/glvertexarray.h"
#include "Wrappers/gltexture.h"
#include "../../../Graphics/texture.h"
#include "glmeshobject.h"
#include "../../../Graphics/mesh.h"
#include "Wrappers/glframebuffer.h"
#include "../../../Graphics/renderstep.h"
#include "glframebuffercontainer.h"


struct GlContextState
{
    bool depthTest = false;
    bool stencilTest = false;
    bool blend = false;
    bool culling = false;
    bool cullFace = false;
};

class GlContext
{
    GlContextState state;
    gl::GlTexture emptyTexture;
    std::unordered_map<std::string, gl::GlMeshObject> meshes;
    std::vector<gl::GlFrameBufferContainer> frameBuffers;

    void blitFrameBuffers(gl::GlFrameBufferContainer& src, gl::GlFrameBufferContainer& target, const FrameBufferProperties& properties,
                          unsigned width, unsigned height);
    void configureTexture(gl::GlTexture& texture, const TextureParameters& parameters);
public:
    gl::GlTexture& LoadTexture(const Texture& texture);
    gl::GlMeshObject& LoadMesh(const Mesh& mesh);
    gl::GlFrameBufferContainer& BuildFrameBuffer(const RenderStep& step, const std::vector<RenderStep>& steps, unsigned viewportWidth, unsigned viewportHeight);
    void UpdateState(const RenderingAttributes& attributes);
    void UpdateFrameBuffer(const RenderingAttributes& attributes);
    void ClearFrameState();
};


#endif //GAMEGL_GLCONTEXT_H
