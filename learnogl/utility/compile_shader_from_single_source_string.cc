// Copyright 2019

#include "utility/utility.h"
#include "glog/logging.h"

GLuint CompileShaderFromSingleSourceString(GLenum shader_type,
                                           const std::string shader_source) {
  GLuint shader_id;
  const char* shader_string = shader_source.c_str();

  shader_id = glCreateShader(shader_type);
  CHECK_EQ(glGetError(), GL_NO_ERROR);

  glShaderSource(shader_id, 1,
                 reinterpret_cast<const GLchar**>(&shader_string),
                 nullptr);
  CHECK_EQ(glGetError(), GL_NO_ERROR);

  glCompileShader(shader_id);

  // Check for compilation error

  int success;
  char infoLog[512];

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
    LOG(FATAL) << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
               << infoLog;
  }

  return shader_id;
}
