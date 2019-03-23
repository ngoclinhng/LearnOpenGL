// Copyright 2018

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

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

#define VERTEX_SHADER_FILE                                              \
  STRINGNIFY(TARGET_OUTPUT_BINARY_DIR) "/colored_triangle.vertex"

#define FRAGMENT_SHADER_FILE                                              \
  STRINGNIFY(TARGET_OUTPUT_BINARY_DIR) "/colored_triangle.fragment"


int main(const int argc, const char* argv[]) {
  // Init GLFW
  GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                    "Colord Triangle", nullptr, nullptr);
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
  const unsigned int num_coordinate_components_per_vertex = 3;
  const unsigned int vertices_offset = 0;
  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,    // left
    0.5f, -0.5f, 0.0f,     // right
    0.0f, 0.5f, 0.0f       // top
  };

  // Colors
  const unsigned int num_color_components_per_vertex = 3;
  GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,  // red
    0.0f, 1.0f, 0.0f,  // green
    0.0f, 0.0f, 1.0f   // blue
  };

  const unsigned int num_components_per_vertex =
      num_coordinate_components_per_vertex * num_color_components_per_vertex;

  // Create the "remember all".
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a buffer (which is nothing fancy rather than jsust an array
  // on GPU to store data).
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               num_vertices * num_components_per_vertex * sizeof(GLfloat),
               nullptr, GL_STATIC_DRAW);

  // Copy data from CPU to GPU
  glBufferSubData(GL_ARRAY_BUFFER, vertices_offset,
                  num_vertices * num_coordinate_components_per_vertex *
                  sizeof(GLfloat), vertices);
  glBufferSubData(GL_ARRAY_BUFFER,
                  num_vertices * num_coordinate_components_per_vertex *
                  sizeof(GLfloat),
                  num_vertices * num_color_components_per_vertex *
                  sizeof(GLfloat), colors);

  // Now we have data in the GPU, how can we tell OpenGL shaders program to
  // fetch these data. Easy!

  // Tell vertex shader how to use position attribute.
  // GLuint vertex_position;
  // vertex_position = glGetAttribLocation(program, "vertex_position");
  // glEnableVertexAttribArray(vertex_position);
  glVertexAttribPointer(0, num_coordinate_components_per_vertex,
                        GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // Tell fragment shader how to use color attribute
  // GLuint vertex_color;
  // vertex_color = glGetAttribLocation(program, "vertex_color");
  // glEnableVertexAttribArray(vertex_color);
  glVertexAttribPointer(1, num_color_components_per_vertex,
                        GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(num_vertices *
                                      num_coordinate_components_per_vertex *
                                      sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glUseProgram(program);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Process input, such as a mouse clicked, a button pressed, etc...
    ProcessUserInput(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
