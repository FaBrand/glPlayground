#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const VertexShader& vs, const FragmentShader& fs)
{
    program_ = glCreateProgram();

    const auto vs_id = dynamic_cast<const Shader&>(vs).GetShaderId();
    const auto fs_id = dynamic_cast<const Shader&>(fs).GetShaderId();

    glAttachShader(program_, vs_id);
    glAttachShader(program_, fs_id);

    glLinkProgram(program_);
    glValidateProgram(program_);

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
}

void ShaderProgram::Bind() const
{
    glUseProgram(program_);
}

void ShaderProgram::Unbind() const
{
    glDeleteProgram(program_);
}
