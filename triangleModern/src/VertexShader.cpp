#include "VertexShader.h"
#include <GLFW/glfw3.h>

VertexShader::VertexShader(const std::string& file_path) : Shader(file_path)
{
    Create();
}

GLenum VertexShader::GetShaderType()
{
    return GL_VERTEX_SHADER;
}
