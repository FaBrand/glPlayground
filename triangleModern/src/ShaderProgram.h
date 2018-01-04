#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <cstring>
#include <map>
#include <GL/glew.h>
#include "FragmentShader.h"
#include "VertexShader.h"

class ShaderProgram
{
  public:
    ShaderProgram(const VertexShader& vs, const FragmentShader& fs);
    void SetUniform(const char* uniform_name, float x, float y, float z, float a);
    void Bind() const;
    void Unbind() const;

  private:
    void AttachShader(const Shader& shader) const;
    void DeleteShader(const Shader& shader) const;
    int GetUniformLocation(const char* uniform_name);
    GLuint program_;

    std::map<std::string, int> uniform_location_map;
};

#endif /* SHADERPROGRAM_H */
