// Copyright 2019

#ifndef SOURCE_UTILITY_INDEX_BUFFER_H_
#define SOURCE_UTILITY_INDEX_BUFFER_H_

#include "utility/common.h"

template<typename IndexType>
class IndexBuffer {
 private:
  GLuint buffer_id_;
  GLuint num_indices_;

 public:
  inline  IndexBuffer(const GLuint num_indices, const GLvoid* indices)
      : num_indices_(num_indices) {
    GL_CALL(glGenBuffers(1, &buffer_id_));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         num_indices * sizeof(IndexType),
                         indices, GL_STATIC_DRAW));
  }

  inline ~IndexBuffer() {
    GL_CALL(glDeleteBuffers(1, &buffer_id_));
  }

  IndexBuffer(const IndexBuffer&) = delete;
  IndexBuffer& operator=(const IndexBuffer&) = delete;

  inline void Bind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_));
  }

  inline void UnBind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

  inline GLuint num_indices() const { return num_indices_; }
};
#endif  // SOURCE_UTILITY_INDEX_BUFFER_H_
