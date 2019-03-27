// Copyright 2018

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

#define VERTEX_SHADER_FILE                                      \
  STRINGNIFY(SHADERS_DESTINATION_DIR) "index_buffer.vertex"

#define FRAGMENT_SHADER_FILE                                    \
  STRINGNIFY(SHADERS_DESTINATION_DIR) "index_buffer.fragment"


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

  // Create shader program from source files
  Shader program(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

  // Vertices
  const unsigned int num_vertices = 6;
  GLfloat vertices[] = {
    0.0f, 0.5f, 0.0f,     // 0
    -0.25f, 0.0f, 0.0f,   // 1
    0.25f, 0.0f, 0.0f,    // 2
    -0.5f, -0.5f, 0.0f,   // 3
    0.0f, -0.5f, 0.0f,    // 4
    0.5f, -0.5f, 0.0f     // 5
  };

  // Colors
  GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,  // 0
    0.0f, 1.0f, 0.0f, 1.0f,  // 1
    0.0f, 0.0f, 1.0f, 1.0f,  // 2
    0.0f, 0.0f, 1.0f, 1.0f,  // 3
    1.0f, 0.0f, 0.0f, 1.0f,  // 4
    0.0f, 1.0f, 0.0f, 1.0f   // 5
  };

  // Indices
  const unsigned int num_indices = 9;
  GLuint indices[] = {0, 1, 2, 1, 3, 4, 2, 4, 5};

  // Create the "remember all".
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a buffer (which is nothing fancy rather than jsust an array
  // on GPU to store data).
  VertexBuffer vbo(num_vertices * 7 * sizeof(GLfloat));

  // Copy data from CPU to GPU
  vbo.SubData(0, num_vertices * 3 * sizeof(GLfloat), vertices);
  vbo.SubData(num_vertices * 3 * sizeof(GLfloat),
              num_vertices * 4 * sizeof(GLfloat),
              colors);

  // Tell vertex shader how to use position attribute.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // Tell fragment shader how to use color attribute
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(num_vertices * 3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // Create index buffer
  IndexBuffer<GLuint> index_buffer(num_indices, indices);

  program.UseProgram();

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Process input, such as a mouse clicked, a button pressed, etc...
    ProcessUserInput(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

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
