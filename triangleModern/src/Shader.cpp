#include "Shader.h"
#include <fstream>
#include <iostream>
#include <memory>

Shader::Shader(const std::string& source_file_path)
{
    std::ifstream shader_file(source_file_path);
    if (!shader_file.is_open() || !shader_file.good())
    {
        std::cout << "Error while opening shader file " << source_file_path << std::endl;
        return;
    }

    source_code_ = std::string(std::istreambuf_iterator<char>(shader_file), std::istreambuf_iterator<char>());
}

GLuint Shader::GetShaderId() const
{
    return shader_id_;
}

void Shader::Create()
{
    // Is called here since the method should not be called from the constructor
    shader_id_ = glCreateShader(GetShaderType());

    const char* source_string = source_code_.c_str();
    constexpr auto source_count{1};
    glShaderSource(shader_id_, source_count, &source_string, nullptr);
    glCompileShader(shader_id_);

    int result;
    glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        int length;
        glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &length);
        auto output = std::make_unique<char[]>(length);

        glGetShaderInfoLog(shader_id_, length, &length, output.get());
        std::cout << "Failed to compile shader!" << std::endl;

        std::cout << output.get() << std::endl;
        glDeleteShader(shader_id_);
        shader_id_ = 0;
    }
}
