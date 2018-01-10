#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <cstdint>

class VertexBuffer
{
  public:
    VertexBuffer(const void* data, std::uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

  private:
    std::uint32_t renderer_id_;
};

#endif /* VERTEXBUFFER_H */
