// Copyright 2019

#ifndef SOURCE_UTILITY_VERTEX_BUFFER_H_
#define SOURCE_UTILITY_VERTEX_BUFFER_H_

#include <glad/glad.h>

class VertexBuffer {
 private:
  GLuint buffer_id_;

 public:
  explicit VertexBuffer(const GLsizeiptr size,
                        const GLvoid* data = nullptr);
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer&) = delete;
  VertexBuffer& operator=(const VertexBuffer&) = delete;

  void SubData(const GLintptr offset, const GLsizeiptr size,
               const GLvoid* data) const;

  void Bind() const;
  void UnBind() const;
};
#endif  // SOURCE_UTILITY_VERTEX_BUFFER_H_
