// Copyright 2019

#include "utility/vertex_buffer.h"
#include "utility/common.h"

VertexBuffer::VertexBuffer(const GLsizeiptr size,
                           const GLvoid* data) {
  GL_CALL(glGenBuffers(1, &buffer_id_));
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer_id_));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  GL_CALL(glDeleteBuffers(1, &buffer_id_));
}

void VertexBuffer::SubData(const GLintptr offset, const GLsizeiptr size,
                           const GLvoid* data) const {
  GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::Bind() const {
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer_id_));
}

void VertexBuffer::UnBind() const {
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
