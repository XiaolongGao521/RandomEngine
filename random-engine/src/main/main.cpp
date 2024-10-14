#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main() {
  glfwInit();

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello, GLFW!", NULL, NULL);
  if (!window) {
      glfwTerminate();
      return -1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {
      // Rendering code here

      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  glfwTerminate();
  
  return 0;
}