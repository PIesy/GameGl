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
#include "../../../Helpers/lock_holder.h"
#include "Wrappers/glprogrampipeline.h"
#include "glprogram.h"
#include "gl_program_handle.h"


struct GlContextState
{
    bool depthTest = false;
    bool stencilTest = false;
    bool blend = false;
    bool culling = false;
    bool cullFace = false;
    bool wireframe = false;
};

struct GlContextData
{
    std::unordered_map<std::string, gl::GlMeshObject> meshes;
    std::unordered_map<size_t, gl::GlProgramPipeline> programPipelines;

    // Per frame
    std::vector<LockedResource<gl::GlTexture>> lockedTextures;
    std::vector<gl::GlFrameBufferContainer> frameBuffers;
    std::vector<gl::GlTexture> tempTextures;
};

class GlContext
{
    GlContextState state;
    GlContextData data;
    int usedDepthTextures = 0;

    void blitFrameBuffers(gl::GlFrameBufferContainer& src, gl::GlFrameBufferContainer& target, const FrameBufferProperties& properties,
                          unsigned width, unsigned height);
    void configureTexture(gl::GlTexture& texture, const TextureParameters& parameters, const TextureInfo& info);
    gl::GlTexture& acquireLocalTexture(const Texture& texture);
public:
    LockedResource<gl::GlTexture> AcquireTexture(const Texture& texture, bool read);
    gl::GlMeshObject& LoadMesh(const Mesh& mesh);
    gl::GlProgramPipeline& GetProgramPipeline(const std::vector<std::reference_wrapper<Shader>>& programs);
    gl::GlFrameBufferContainer& BuildFrameBuffer(const RenderStep& step, const std::vector<RenderStep>& steps, unsigned viewportWidth, unsigned viewportHeight);
    void UpdateState(const RenderingAttributes& attributes);
    void UpdateFrameBuffer(const RenderingAttributes& attributes);
    void UpdateFrameState(gl::GlFrameBufferContainer& frameBuffer, const RenderingAttributes& attributes);
    void ClearFrameState();
};


#endif //GAMEGL_GLCONTEXT_H
