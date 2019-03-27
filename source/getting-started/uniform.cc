// Copyright 2018

#include <cmath>

#include "utility/gl.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessUserInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

#ifndef SHADERS_DESTINATION_DIR
#error SHADERS_DESTINATION_DIR is undefined
#endif

#if defined(VERTEX_SHADER_FILE)
#undef VERTEX_SHADER_FILE
#endif

#if defined(FRAGMENT_SHADER_FILE)
#undef FRAGMENT_SHADER_FILE
#endif

#define VERTEX_SHADER_FILE \
  STRINGNIFY(SHADERS_DESTINATION_DIR) "uniform.vertex"

#define FRAGMENT_SHADER_FILE                                    \
  STRINGNIFY(SHADERS_DESTINATION_DIR) "uniform.fragment"


int main(const int argc, const char* argv[]) {
  // Init GLFW
  GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                    "Uniform", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  // Init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG(INFO) << "Failed to initialize GLAD";
    glfwTerminate();
    return -1;
  }

  Shader program(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

  // Vertices
  const unsigned int num_vertices = 3;
  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,    // left
    0.5f, -0.5f, 0.0f,     // right
    0.0f, 0.5f, 0.0f       // top
  };

  // Create the "remember all".
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a buffer (which is nothing fancy rather than jsust an array
  // on GPU to store data).
  VertexBuffer vbo(num_vertices * 3 * sizeof(GLfloat), vertices);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Process input, such as a mouse clicked, a button pressed, etc...
    ProcessUserInput(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use program
    program.UseProgram();

    // Update uniform color
    GLfloat time = glfwGetTime();
    GLfloat green_value = std::sinf(time) / 2.0f + 0.5f;
    program.SetUniform4f("vertex_color", 0.0f, green_value, 0.0f, 1.0f);

    // Draw
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &vao);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
void ProcessUserInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this
// callback function executes.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions;
  // note that width and  height will be significantly larger than specified
  // on retina displays.
  glViewport(0, 0, width, height);
}
