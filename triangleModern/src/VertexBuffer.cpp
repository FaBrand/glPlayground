#include "VertexBuffer.h"
#include <GL/glew.h>

constexpr auto BUFFER_TYPE = GL_ARRAY_BUFFER;

VertexBuffer::VertexBuffer(const void* data, std::uint32_t size)
{
    glGenBuffers(1, &renderer_id_);
    glBindBuffer(BUFFER_TYPE, renderer_id_);
    glBufferData(BUFFER_TYPE, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &renderer_id_);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(BUFFER_TYPE, renderer_id_);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(BUFFER_TYPE, 0);
}
