//
// Created by akann on 9/7/17.
//

#ifndef GAMEGL_GLSHADER_H
#define GAMEGL_GLSHADER_H

#include "glbindings.h"
#include "globject.h"

namespace gl
{

    enum class GlShaderType : GLenum
    {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        TesselationControl = GL_TESS_CONTROL_SHADER,
        TesselationEvaluation = GL_TESS_EVALUATION_SHADER,
        Compute = GL_COMPUTE_SHADER,
        Undefined
    };

    class GlShader : public GlObject
    {
        GLuint shader;
    public:
        explicit GlShader(GlShaderType type);
        GlShader(const GlShader&) = delete;
        GlShader(GlShader&& rhs) noexcept;
        ~GlShader() override;

        void SetSource(const std::string& source);
        GLuint GetId() const override;
    };

}

#endif //GAMEGL_GLSHADER_H
