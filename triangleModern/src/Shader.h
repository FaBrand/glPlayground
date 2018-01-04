#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
  public:
    Shader(const std::string& source_file_path);
    ~Shader() = default;

    GLuint GetShaderId() const;

  protected:
    virtual GLenum GetShaderType() = 0;
    void Create();

  private:
    GLuint shader_id_{0};
    std::string source_code_{};
};

#endif /* SHADER_H */
