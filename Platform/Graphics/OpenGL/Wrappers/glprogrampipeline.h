//
// Created by akann on 9/13/17.
//

#ifndef GAMEGL_GLPIPLINE_H
#define GAMEGL_GLPIPLINE_H

#include "glbindings.h"
#include "glprogram.h"
#include <vector>
#include <string>

namespace gl
{

    enum class GlProgramBit : GLbitfield
    {
        VertexShader = GL_VERTEX_SHADER_BIT,
        FragmentShader = GL_FRAGMENT_SHADER_BIT,
        GeometryShader = GL_GEOMETRY_SHADER_BIT,
        TesselationControl = GL_TESS_CONTROL_SHADER_BIT,
        TesselationEvaluation = GL_TESS_EVALUATION_SHADER_BIT,
        Compute = GL_COMPUTE_SHADER_BIT,
        Error
    };

    class GlProgramPipeline :  public GlObject
    {
        GLuint programPipeline;
    public:
        explicit GlProgramPipeline(const std::vector<std::reference_wrapper<const GlProgram>>& programs);
        GlProgramPipeline(const GlProgramPipeline&) = delete;
        GlProgramPipeline(GlProgramPipeline&& source) noexcept;
        GlProgramPipeline& operator=(GlProgramPipeline&& source) noexcept;
        ~GlProgramPipeline() override;
        void Bind();
        void Unbind();
        void Validate();
        std::string GetInfo();

        GLuint GetId() const override;
    };

}


#endif //GAMEGL_GLPIPLINE_H
