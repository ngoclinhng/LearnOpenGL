// Copyright 2019

#ifndef SOURCE_UTILITY_COMMON_H_
#define SOURCE_UTILITY_COMMON_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glog/logging.h"

// Helper to convert macro to string
#define STRINGNIFY(x)  STRINGNIFY_HELPER(x)
#define STRINGNIFY_HELPER(x) #x

// Helper macro to offset into buffer
#define BUFFER_OFFSET(offset) (reinterpret_cast<char*>(NULL) + (offset))


// Simple utility used to debug OpenGL function when Debug mode is on

inline void GLLogCall(const char* function_name, const char* file_name,
                      int line_number) {
  GLenum error;
  while ((error = glGetError())) {
    LOG(FATAL) << "[OpenGL error] (" << error << "): " << function_name
               << ", in file: " << file_name << ", at line number: "
               << line_number;
  }
}

#if defined(NDEBUG)
#define GL_CALL(SomeOpenGLFunction)  SomeOpenGLFunction;
#else
#define GL_CALL(SomeOpenGLFunction) {                   \
    while (glGetError() != GL_NO_ERROR) {}              \
    SomeOpenGLFunction;                                 \
    GLLogCall(#SomeOpenGLFunction, __FILE__, __LINE__); \
  }
#endif


// Initialize and create GLFW window.
GLFWwindow* CreateWindow(int width, int height, const char* title,
                         GLFWmonitor* monitor, GLFWwindow* share);

#endif  // SOURCE_UTILITY_COMMON_H_
