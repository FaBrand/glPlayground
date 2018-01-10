#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <cstdint>

class IndexBuffer
{
  public:
    IndexBuffer(const void* data, std::uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

  private:
    std::uint32_t renderer_id_;
    std::uint32_t count_;
};

#endif /* INDEXBUFFER_H */
