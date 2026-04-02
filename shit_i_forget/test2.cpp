#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    int fuck = 54;
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLAD Test Window", NULL, NULL);
    if (window == NULL) {
      std::cerr << "Failed to create GLFW window\n";
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD by passing it the GLFW OS-specific pointer function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD\n";
      return -1;
    }

    // If we make it here, the bridge is up! Let's print the specs to the terminal.
    std::cout << "GLAD initialized successfully!\n";
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    glViewport(0, 0, 800, 600);

    // The Render Loop
    while (!glfwWindowShouldClose(window)) {
        
      // Calculate a pulsating green value based on time
      double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        
        glClearColor(0.0f, greenValue, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
