//
// Created by akann on 9/13/17.
//

#include "glprogrampipeline.h"
#include "../../../../Logger/logger.h"
#include "../../../../Helpers/helpers.h"

gl::GlProgramBit convertShaderType(gl::GlShaderType type);

const auto logger = Logger::GetLogger(getClassName<gl::GlProgramPipeline>());

gl::GlProgramBit convertShaderType(gl::GlShaderType type)
{
    switch (type)
    {
        case gl::GlShaderType::Vertex:
            return gl::GlProgramBit::VertexShader;
        case gl::GlShaderType::Fragment:
            return gl::GlProgramBit::FragmentShader;
        case gl::GlShaderType::Geometry:
            return gl::GlProgramBit::GeometryShader;
        case gl::GlShaderType::TesselationControl:
            return gl::GlProgramBit::TesselationControl;
        case gl::GlShaderType::TesselationEvaluation:
            return gl::GlProgramBit::TesselationEvaluation;
        case gl::GlShaderType::Compute:
            return gl::GlProgramBit::Compute;
        case gl::GlShaderType::Undefined:
            logger.LogError("Unsupported program object cannot be bound to pipeline");
            return gl::GlProgramBit::Error;
    }
}

gl::GlProgramPipeline::GlProgramPipeline(const std::vector<std::reference_wrapper<const gl::GlProgram>>& programs)
{
    programPipeline = gl::program_pipeline::create();

    for (const gl::GlProgram& program : programs)
        gl::program_pipeline::useProgramStages(programPipeline, integral(convertShaderType(program.GetType())), program.GetId());
}

gl::GlProgramPipeline::~GlProgramPipeline()
{
    gl::program_pipeline::erase(programPipeline);
}

GLuint gl::GlProgramPipeline::GetId() const
{
    return programPipeline;
}

gl::GlProgramPipeline::GlProgramPipeline(gl::GlProgramPipeline&& source) noexcept
{
    *this = std::move(source);
}

gl::GlProgramPipeline& gl::GlProgramPipeline::operator=(gl::GlProgramPipeline&& source) noexcept
{
    programPipeline = source.programPipeline;
    source.programPipeline = 0;
    return *this;
}

void gl::GlProgramPipeline::Bind()
{
    gl::program_pipeline::bindProgramPipeline(programPipeline);
}

void gl::GlProgramPipeline::Unbind()
{
    gl::program_pipeline::bindProgramPipeline(0);
}

void gl::GlProgramPipeline::Validate()
{
    gl::program_pipeline::validate(programPipeline);
}

std::string gl::GlProgramPipeline::GetInfo()
{
    return gl::program_pipeline::getInfoLog(programPipeline);
}
