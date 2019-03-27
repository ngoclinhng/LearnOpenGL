// Copyright 2018.

#include "utility/common.h"

GLFWwindow* CreateWindow(int width, int height, const char* title,
                         GLFWmonitor* monitor, GLFWwindow* share) {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, share);
  CHECK_NOTNULL(window);
  return window;
}
