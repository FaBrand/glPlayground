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

void ShaderProgram::SetUniform(const char* uniform_name, float x, float y, float z, float a)
{
    const auto uniform_location{GetUniformLocation(uniform_name)};
    glUniform4f(uniform_location, x, y, z, a);
}

int ShaderProgram::GetUniformLocation(const char* uniform_name)
{
    if (uniform_location_map.find(uniform_name) == uniform_location_map.end())
    {
        int uniform_location{glGetUniformLocation(program_, uniform_name)};
        uniform_location_map[std::string(uniform_name)] = uniform_location;
    }

    return uniform_location_map.at(uniform_name);
}
