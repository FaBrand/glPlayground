#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const VertexShader& vs, const FragmentShader& fs)
{
    program_ = glCreateProgram();

    AttachShader(vs);
    AttachShader(fs);

    glLinkProgram(program_);
    glValidateProgram(program_);

    DeleteShader(vs);
    DeleteShader(fs);
}

void ShaderProgram::AttachShader(const Shader& shader) const
{
    glAttachShader(program_, shader.GetShaderId());
}

void ShaderProgram::DeleteShader(const Shader& shader) const
{
    glDeleteShader(shader.GetShaderId());
}

void ShaderProgram::Bind() const
{
    glUseProgram(program_);
}

void ShaderProgram::Unbind() const
{
    glDeleteProgram(program_);
}
