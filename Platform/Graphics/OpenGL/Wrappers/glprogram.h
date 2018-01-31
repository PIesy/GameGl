//
// Created by akann on 9/7/17.
//

#ifndef GAMEGL_GLPROGRAM_H
#define GAMEGL_GLPROGRAM_H

#include "globject.h"
#include "glshader.h"

namespace gl
{

    class GlProgram : public GlObject
    {
        GLuint program;
        const GlShaderType type = GlShaderType::Undefined;
    public:
        GlProgram();
        GlProgram(GlShaderType type, const std::string& source);
        GlProgram(const GlProgram&) = delete;
        GlProgram(GlProgram&& src) noexcept;
        ~GlProgram() override;

        void AttachShader(GlShader& shader);
        void DetachShader(GlShader& shader);
        void Link();
        void Use();
        GLint GetUniformLocation(const std::string& name);

        void SetFloatVectorUniform(GLuint location, short vectorSize, GLfloat* values, unsigned count);
        void SetIntVectorUniform(GLuint location, short vectorSize, GLint* values, unsigned count);
        void SetUnsignedVectorUniform(GLuint location, short vectorSize, GLuint* values, unsigned count);
        void SetDoubleVectorUniform(GLuint location, short vectorSize, GLdouble* values, unsigned count);
        void SetFloatMatrixUniform(GLuint location, short horizontalSize, short verticalSize, GLfloat* values, unsigned count);
        void SetDoubleMatrixUniform(GLuint location, short horizontalSize, short verticalSize, GLdouble* values, unsigned count);
        GlShaderType GetType() const;
        GLuint GetId() const override;
    };

}

#endif //GAMEGL_GLPROGRAM_H
