#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include "FragmentShader.h"
#include "VertexShader.h"

class ShaderProgram
{
  public:
    ShaderProgram(const VertexShader& vs, const FragmentShader& fs);
    void Bind() const;
    void Unbind() const;

  private:
    void AttachShader(const Shader& shader) const;
    void DeleteShader(const Shader& shader) const;
    GLuint program_;
};

#endif /* SHADERPROGRAM_H */
