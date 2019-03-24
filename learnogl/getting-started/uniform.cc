// Copyright 2018

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cmath>

#include "utility/utility.h"
#include "glog/logging.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessUserInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

#ifndef TARGET_OUTPUT_BINARY_DIR
#error TARGET_OUTPUT_BINARY_DIR is undefined
#endif

#if defined(VERTEX_SHADER_FILE)
#undef VERTEX_SHADER_FILE
#endif

#if defined(FRAGMENT_SHADER_FILE)
#undef FRAGMENT_SHADER_FILE
#endif

#define VERTEX_SHADER_FILE \
  STRINGNIFY(TARGET_OUTPUT_BINARY_DIR) "/uniform.vertex"

#define FRAGMENT_SHADER_FILE                                    \
  STRINGNIFY(TARGET_OUTPUT_BINARY_DIR) "/uniform.fragment"


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

  // Load shaders source
  std::string vertex_string = ReadFileContent(VERTEX_SHADER_FILE);
  std::string fragment_string = ReadFileContent(FRAGMENT_SHADER_FILE);

  // Compile shaders
  GLuint vertex_shader =
      CompileShaderFromSingleSourceString(GL_VERTEX_SHADER, vertex_string);
  GLuint fragment_shader =
      CompileShaderFromSingleSourceString(GL_FRAGMENT_SHADER, fragment_string);

  // Create program
  GLuint program = CreateProgramFromShaders(vertex_shader, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

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
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // Uniform color
  GLuint vertex_color_id;

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Process input, such as a mouse clicked, a button pressed, etc...
    ProcessUserInput(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use program
    glUseProgram(program);

    // Update uniform color
    GLfloat time = glfwGetTime();
    GLfloat green_value = std::sinf(time) / 2.0f + 0.5f;
    vertex_color_id = glGetUniformLocation(program, "vertex_color");
    glUniform4f(vertex_color_id, 0.0f, green_value, 0.0f, 1.0f);

    // Draw
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

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
