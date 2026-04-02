#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

// THE SHADERS (Unchanged from before)
const char* vertexShaderSource = 
    "#version 460 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform vec2 offset;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, 0.0, 1.0);\n"
    "}\n";

const char* fragmentShaderSource = 
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 shapeColor;\n"
    "void main() {\n"
    "   FragColor = vec4(shapeColor, 1.0);\n"
    "}\n";

// NEW: A blueprint for an Asteroid in our C++ RAM
struct Asteroid {
    float x, y;       // Current position
    float dx, dy;     // Velocity (Speed and direction)
    float r, g, b;    // Individual color
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Asteroid Swarm", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 1. BUILD THE RUBBER STAMP (The Circle VBO)
    std::vector<float> circVertices;
    std::vector<unsigned int> circIndices;
    circVertices.push_back(0.0f); 
    circVertices.push_back(0.0f); 
    std::cout << "hellow world";
    std::cout << "fuck you all";

    
    int segments = 18; // Slightly lower resolution so 100 circles run faster
    float radius = 0.05f; // Make them smaller!
    for (int i = 0; i <= segments; i++) {
        float angle = i * (2.0f * 3.14159f / segments);
        circVertices.push_back(cos(angle) * radius);
        circVertices.push_back(sin(angle) * radius);
        if (i > 0) {
            circIndices.push_back(0);
            circIndices.push_back(i);
            circIndices.push_back(i + 1);
        }
    }
    unsigned int vaoCirc, vboCirc, eboCirc;
    glGenVertexArrays(1, &vaoCirc);
    glGenBuffers(1, &vboCirc);
    glGenBuffers(1, &eboCirc);

    glBindVertexArray(vaoCirc);
    glBindBuffer(GL_ARRAY_BUFFER, vboCirc);
    glBufferData(GL_ARRAY_BUFFER, circVertices.size() * sizeof(float), circVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboCirc);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, circIndices.size() * sizeof(unsigned int), circIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 2. CREATE THE SWARM IN C++ RAM
    std::vector<Asteroid> swarm;
    int numAsteroids = 100;
    
    for(int i = 0; i < numAsteroids; i++) {
        Asteroid a;
        // Random starting positions between -0.8 and 0.8
		a.x = ((rand() % 160) / 100.0f) - 0.8f; 
        a.y = ((rand() % 160) / 100.0f) - 0.8f;
        // Random velocities
        a.dx = ((rand() % 100) / 10000.0f) - 0.005f; 
        a.dy = ((rand() % 100) / 10000.0f) - 0.005f;
        // Random neon colors
        a.r = (rand() % 100) / 100.0f;
        a.g = (rand() % 100) / 100.0f;
        a.b = 1.0f; // Keep blue high so they look cool
        
        swarm.push_back(a);
    }

    glUseProgram(shaderProgram); 
    int offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    int colorLocation = glGetUniformLocation(shaderProgram, "shapeColor");

    // 3. THE RENDER LOOP
    while (!glfwWindowShouldClose(window)) {
        
        // Clean the hidden canvas
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark space background
        glClear(GL_COLOR_BUFFER_BIT);

        // Bring the Circle Boss into the room ONCE
        glBindVertexArray(vaoCirc);

        // Loop through all 100 asteroids in our C++ vector
        for (int i = 0; i < (int)swarm.size(); i++) {
            
            // MATH: Update the position based on velocity
            swarm[i].x += swarm[i].dx;
            swarm[i].y += swarm[i].dy;

            // PHYSICS: Bounce off the walls (Window edges are -1.0 to +1.0)
            if (swarm[i].x > 1.0f || swarm[i].x < -1.0f) swarm[i].dx *= -1;
            if (swarm[i].y > 1.0f || swarm[i].y < -1.0f) swarm[i].dy *= -1;

            // GPU COMMAND: Move the Uniform Hand to the new C++ position
            glUniform2f(offsetLocation, swarm[i].x, swarm[i].y);
            
            // GPU COMMAND: Change the Uniform Color for this specific asteroid
            glUniform3f(colorLocation, swarm[i].r, swarm[i].g, swarm[i].b);
            
            // GPU COMMAND: Stamp it on the hidden canvas!
            glDrawElements(GL_TRIANGLES, circIndices.size(), GL_UNSIGNED_INT, 0);
            
            /*
            const char* vertexShaderSource = 
			"#version 460 core\n"
			"layout (location = 0) in vec2 aPos;\n"
			"uniform vec2 offset;\n"
			"void main() {\n"
			"   gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, 0.0, 1.0);\n"
			"}\n";
            */
        }

        // Swap the hidden canvas to the monitor
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
