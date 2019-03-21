// Copyright 2019

#ifndef LEARNOGL_UTILITY_UTILITY_H_
#define LEARNOGL_UTILITY_UTILITY_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <type_traits>

#include "glog/logging.h"

// Helper to convert macro to string
#define STRINGNIFY(x)  STRINGNIFY_HELPER(x)
#define STRINGNIFY_HELPER(x) #x

// Type trait to check whether a type is actually GLuint.
template <typename T> struct is_gluint : public std::false_type {};
template<> struct is_gluint<GLuint> : public std::true_type {};

// Given the path to a file, reads the content, and returns the file content
// as string
std::string ReadFileContent(const char* file_name);

// Given the paths to the vertex shader, and fragment shader, builds the
// program, and returns the program ID.
GLuint BuildProgramFromShaders(const char* vertex_shader_file,
                               const char* fragment_shader_file);

// Initialize and create GLFW window.
GLFWwindow* CreateWindow(int width, int height, const char* title,
                         GLFWmonitor* monitor, GLFWwindow* share);

// Create and compile shader program from a single source code stored
// as c string.
// Return an ID for this program to be used.
GLuint CompileShaderFromSingleSourceString(GLenum shader_type,
                                           const char* shader_source);

// Build program from several shaders.
// See [1] for the technique be used here. I know, i know, this is a bit
// overkilled, but...we're learning, aren't we?
// [1] - https://wiki.sei.cmu.edu/confluence/display/cplusplus/DCL50-CPP.+Do+not+define+a+C-style+variadic+function
template<typename Arg, typename... Ts,
         typename std::enable_if<is_gluint<Arg>::value>::type * = nullptr>
GLuint CreateProgramFromShaders(Arg shader_id, Ts... rest) {
  GLuint program = glCreateProgram();
  Arg shaders[] = { rest... };

  glAttachShader(program, shader_id);

  for (auto shader : shaders) {
    glAttachShader(program, shader);
  }

  glLinkProgram(program);

  // Check for linking error

  int success;
  char infoLog[512];

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    LOG(FATAL) << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
               << infoLog;
  }
  return program;
}

#endif  // LEARNOGL_UTILITY_UTILITY_H_
