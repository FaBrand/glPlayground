#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include <cstring>
#include <GL/glew.h>
#include "Shader.h"

class FragmentShader : public Shader
{
  public:
    FragmentShader(const std::string& file_path);
    GLenum GetShaderType() override;
};

#endif /* FRAGMENTSHADER_H */
