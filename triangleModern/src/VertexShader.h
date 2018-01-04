#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include <string>
#include <GL/glew.h>
#include "Shader.h"

class VertexShader : public Shader
{
  public:
    VertexShader(const std::string& file_path);
    GLenum GetShaderType() override;
};

#endif /* VERTEXSHADER_H */
