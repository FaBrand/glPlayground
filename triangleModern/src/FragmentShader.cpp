#include "FragmentShader.h"
#include <GLFW/glfw3.h>

FragmentShader::FragmentShader(const std::string& file_path) : Shader(file_path)
{
    Create();
}

GLenum FragmentShader::GetShaderType()
{
    return GL_FRAGMENT_SHADER;
}
