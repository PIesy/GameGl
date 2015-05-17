#include "renderdefs.h"

GLuint convertToGlShaderType(ShaderType type)
{
    switch(type)
    {
    case ShaderType::VertexShader:
        return GL_VERTEX_SHADER;
    case ShaderType::FragmentShader:
        return GL_FRAGMENT_SHADER;
    }
}
