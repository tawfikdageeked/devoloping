a#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// initializing GLFW (the window manager) and giving it hints to what version of opengl are we using
int InitializeGLFW(int major_ver, int minor_ver)
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return 0;
}

// one already declared window i won't need more than one window
GLFWwindow* window;

int CreateWindow(int ScreenWidth, int ScreenHeight, std::string WindowTag)
{
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, WindowTag.c_str(), NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

// initialize glew
// why glew not glad cuz those files will be compiled on multible platforms so glew is way easier to deal with
int InitializeGLEW()
{
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    
    return 0;
}

// opengl give everything an int to make is to use stuff
unsigned int vertShader;
unsigned int fragShader;
unsigned int mainProgram;

// create the vertex shader in one call
int CreateVertShader(const std::string& filepath)
{
    std::ifstream file(filepath);

    if(!file.is_open())
    {
        std::cout << "Failed to open vertex shader file\n";
        return -1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string VertexShaderSource = buffer.str();
    const char* src = VertexShaderSource.c_str();

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &src, NULL);
    glCompileShader(vertShader);

    int success;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
        std::cout << "VERTEX SHADER COMPILATION FAILED\n" << infoLog << "\n";
        return -1;
    }

    return vertShader;
}

// create the fragment shader in one call
int CreateFragShader(const std::string& filepath)
{
    std::ifstream file(filepath);

    if(!file.is_open())
    {
        std::cout << "Failed to open fragment shader file\n";
        return -1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string FragmentShaderSource = buffer.str();
    const char* src = FragmentShaderSource.c_str();

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &src, NULL);
    glCompileShader(fragShader);

    int success;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << "\n";
        return -1;
    }
    
    return fragShader;
}

// create the main program in one call
unsigned int CreateProgram()
{
    mainProgram = glCreateProgram();
    glAttachShader(mainProgram, vertShader);
    glAttachShader(mainProgram, fragShader);
    glLinkProgram(mainProgram);

    int success;
    glGetProgramiv(mainProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(mainProgram, 512, nullptr, infoLog);
        std::cout << "SHADER PROGRAM LINKING FAILED\n" << infoLog << "\n";
        return 0;
    }

    // Clean up the individual shaders once they are safely linked
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return mainProgram;
}


void UseProgram(unsigned int program)
{
    glUseProgram(program);
    return;
}
