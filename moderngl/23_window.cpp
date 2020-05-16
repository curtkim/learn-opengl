#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "moderngl.hpp"
#include <shader_s.h>

class WindowExample : public Example{

public:

  ~WindowExample(){}

  GLFWframebuffersizefun getFramebufferSizeCallback() {
    return [](GLFWwindow* window, int width, int height){
      glViewport(0, 0, width, height);
    };
  }

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(GLFWwindow *window)
  {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  }

  void render(Shader shader) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};

int main()
{
  WindowExample example;
  return runExample(&example, nullptr);
}

