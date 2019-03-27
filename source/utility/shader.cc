// Copyright 2019

#include <string>
#include <fstream>
#include <iterator>

#include "utility/common.h"
#include "utility/shader.h"
#include "glog/logging.h"

namespace {

// Given the path to a file, reads the content, and returns the file content
// as string
std::string ReadFileContent(const char* file_name) {
  std::ifstream file_stream(file_name, std::ifstream::in);
  LOG_IF(FATAL, !file_stream) << "File does not exist: " << file_name;
  std::string file_content((std::istreambuf_iterator<char>(file_stream)),
                           (std::istreambuf_iterator<char>()));
  return file_content;
}

// Create and compile shader program from a single source code stored
// as c string.
// Return an ID for this program to be used.
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

    std::string message;

    if (shader_type == GL_VERTEX_SHADER) {
      message = "Failed to compile vertex shader:\n";
    } else if (shader_type == GL_FRAGMENT_SHADER) {
      message = "Failed to compile fragment shader:\n";
    } else {
      message = "Failed to compile unknown shader type:\n";
    }

    LOG(FATAL) << message << infoLog;
  }

  return shader_id;
}


}  // namespace

Shader::Shader(const char* vertex_shader_file,
               const char* fragment_shader_file) {
  // Load shaders source
  std::string vertex_string = ReadFileContent(vertex_shader_file);
  std::string fragment_string = ReadFileContent(fragment_shader_file);

  // Compile shaders
  GLuint vertex_shader =
      CompileShaderFromSingleSourceString(GL_VERTEX_SHADER, vertex_string);
  GLuint fragment_shader =
      CompileShaderFromSingleSourceString(GL_FRAGMENT_SHADER, fragment_string);

  // Create program
  GLuint program = glCreateProgram();
  CHECK_EQ(glGetError(), GL_NO_ERROR);

  GL_CALL(glAttachShader(program, vertex_shader));
  GL_CALL(glAttachShader(program, fragment_shader));
  GL_CALL(glLinkProgram(program));

  // Check for linking error

  int success;
  char infoLog[512];

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    LOG(FATAL) << "Failed to link shader program:\n" << infoLog;
  }

  id_ = program;
  GL_CALL(glDeleteShader(vertex_shader));
  GL_CALL(glDeleteShader(fragment_shader));
}

Shader::~Shader() {}

void Shader::UseProgram() const {
  GL_CALL(glUseProgram(id_));
}

GLuint Shader::GetAttribLocation(const char* attrib_name) const {
  GLuint location = glGetAttribLocation(id_, attrib_name);
  return location;
}

GLuint Shader::GetUniformLocation(const char* uniform_name) const {
  GLuint location = glGetUniformLocation(id_, uniform_name);
  CHECK_EQ(glGetError(), GL_NO_ERROR);
  return location;
}

void Shader::SetUniform4f(const char* uniform_name,
                          const GLfloat v0,
                          const GLfloat v1,
                          const GLfloat v2,
                          const GLfloat v3) const {
  GLuint location = GetUniformLocation(uniform_name);
  GL_CALL(glUniform4f(location, v0, v1, v2, v3));
}

