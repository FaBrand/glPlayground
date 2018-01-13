#include "IndexBuffer.h"
#include <GL/glew.h>

constexpr auto BUFFER_TYPE = GL_ELEMENT_ARRAY_BUFFER;

IndexBuffer::IndexBuffer(const void* data, std::uint32_t count) : count_(count)
{
    glGenBuffers(1, &renderer_id_);
    glBindBuffer(BUFFER_TYPE, renderer_id_);

    static_assert(sizeof(std::uint32_t) == sizeof(std::uint32_t), "GLuint size not equal to std::uint32_t");
    glBufferData(BUFFER_TYPE, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &renderer_id_);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(BUFFER_TYPE, renderer_id_);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(BUFFER_TYPE, 0);
}
