#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <tuple>

#include "shader_s.h"
#include "camera.h"
#include "utils.hpp"


const char *VERTEX_SHADER_SOURCE = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

const char *FRAGMENT_SHADER_SOURCE = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture0;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(texture0, TexCoord);
}
)";


struct MyHandler {
  // settings
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  // camera
  Camera camera;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;

  // timing
  float deltaTime = 0.0f;  // time between current frame and last frame

  MyHandler() : camera(glm::vec3(0.0f, 0.0f, 3.0f)) {};

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
  }

  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  // ---------------------------------------------------------------------------------------------
  void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
  }

  // glfw: whenever the mouse moves, this callback is called
  // -------------------------------------------------------
  void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
  }

  // glfw: whenever the mouse scroll wheel scrolls, this callback is called
  // ----------------------------------------------------------------------
  void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
  }

  void bindToWindow(GLFWwindow *window) {
    // bind window callback
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        auto &myContext = *static_cast<MyHandler *>(glfwGetWindowUserPointer(window));
        myContext.framebuffer_size_callback(window, width, height);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        auto &myContext = *static_cast<MyHandler *>(glfwGetWindowUserPointer(window));
        myContext.mouse_callback(window, xpos, ypos);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto &myContext = *static_cast<MyHandler *>(glfwGetWindowUserPointer(window));
        myContext.scroll_callback(window, xoffset, yoffset);
    });

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  }
};


auto make_vertex_array() {
  unsigned int VAO, VBO, EBO;
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  const float vertices[] = {
    // positions          // colors           // texture coords
    2.5f, 2.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    2.5f, -2.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -2.5f, -2.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -2.5f, 2.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
  };

  const unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // 1. vertex
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // 2. index
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  return std::make_tuple(VAO, VBO, EBO);
}

int main() {

  MyHandler myHandler;

  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(myHandler.SCR_WIDTH, myHandler.SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  myHandler.bindToWindow(window);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  glEnable(GL_DEPTH_TEST);

  // model
  auto[VAO, VBO, EBO] = make_vertex_array();

  // texture
  unsigned int texture0 = load_texture("resources/textures/container.jpg", true, GL_RGB);
  std::cout << texture0 << " " << GL_TEXTURE0 << std::endl;

  // shader
  Shader ourShader(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);

  float lastFrame = 0;
  // render loop
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    myHandler.deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // std::cout << "deltaTime " << deltaTime << " lastFrame " << lastFrame << std::endl;

    // input
    myHandler.processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    // activate shader
    ourShader.use();
    ourShader.setInt("texture0", 0);

    // projection
    glm::mat4 projection = glm::perspective(glm::radians(myHandler.camera.Zoom),
                                            (float) myHandler.SCR_WIDTH / (float) myHandler.SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // camera view transformation
    ourShader.setMat4("view", myHandler.camera.GetViewMatrix());

    // model
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    ourShader.setMat4("model", model);

    // render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}
