// Copyright 2018

#ifndef SOURCE_UTILITY_SHADER_H_
#define SOURCE_UTILITY_SHADER_H_

#include <glad/glad.h>

class Shader {
 private:
  GLint id_;

 public:
  Shader(const char* vertex_shader_file, const char* fragment_shader_file);
  ~Shader();

  void UseProgram() const;

  GLuint GetAttribLocation(const char* atrrib_name) const;
  GLuint GetUniformLocation(const char* uniform_name) const;

  void SetUniform4f(const char* uniform_name,
                    const GLfloat v0,
                    const GLfloat v1,
                    const GLfloat v2,
                    const GLfloat v3) const;
};
#endif  // SOURCE_UTILITY_SHADER_H_
