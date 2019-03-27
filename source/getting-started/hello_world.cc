// Copyright 2018

#include "utility/gl.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessUserInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#ifndef SHADERS_DESTINATION_DIR
#error SHADERS_DESTINATION_DIR is undefied
#endif

#if defined(VERTEX_SHADER_FILE)
#undef VERTEX_SHADER_FILE
#endif

#if defined(FRAGMENT_SHADER_FILE)
#undef FRAGMENT_SHADER_FILE
#endif

#define VERTEX_SHADER_FILE                                      \
  STRINGNIFY(SHADERS_DESTINATION_DIR) "hello_world.vertex"

#define FRAGMENT_SHADER_FILE                                    \
  STRINGNIFY(SHADERS_DESTINATION_DIR) "hello_world.fragment"

int main() {
  // Create GLFW window
  GLFWwindow* window = CreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                    "Learn OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG(INFO) << "Failed to initialize GLAD";
    glfwTerminate();
    return -1;
  }

  // Create shader program from source files
  Shader program(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,  // left
    0.5f, -0.5f, 0.0f,  // right
    0.0f,  0.5f, 0.0f  // top
  };

  // Create the "remember all"
  // Ask OpenGL to give us exactly one vertex array object.
  // Remember to bind this shit before drawing!!!
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a buffer (which is nothing fancy rather than just an array
  // in GPU) in order to copy data from CPU to GPU, and activate it.
  // (or well bind it if you like).
  // Again we ask GPU to give us exactly one buffer.
  VertexBuffer vbo(3 * 3 * sizeof(GLfloat), vertices);

  // Now all the data is copied from CPU to GPU, we need to tell our shader
  // program how to use this data.
  // To do this, we need to find the position of variable in our shader
  // program.
  GLuint positionID;
  positionID = program.GetAttribLocation("vertex_position");

  // And tell it how to fetch the data from our buffer.
  // In this case, we have only one variable (marked as in) in the
  // vertex shader program.
  glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE,
                        3 * sizeof(GLfloat),
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(positionID);

  // You can unbind the VAO afterwards so other VAO calls won't
  // accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally
  // don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);


  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Process input, such as a mouse clicked, a button pressed, etc...
    ProcessUserInput(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw
    program.UseProgram();
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
