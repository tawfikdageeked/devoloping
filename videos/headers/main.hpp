#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <EGL/egl.h>
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

bool InitializeHeadlessRendering(int width, int height) 
{
    // 1. HIJACK THE LINUX ENVIRONMENT (Force deep logging and explicit vendor paths)
    // The '1' means overwrite existing variables if they exist.
    setenv("EGL_LOG_LEVEL", "debug", 1);
    setenv("LIBGL_DEBUG", "verbose", 1); 
    setenv("__EGL_VENDOR_LIBRARY_FILENAMES", "/usr/share/glvnd/egl_vendor.d/10_nvidia.json", 1);

    std::cout << "--- Booting EGL ---" << std::endl;

    // 2. Connect directly to the default GPU display driver
    EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDpy == EGL_NO_DISPLAY) {
        std::cout << "[FATAL] eglGetDisplay failed!" << std::endl;
        std::cout << "Reason: " << GetEGLErrorString(eglGetError()) << std::endl;
        return false;
    }

    // 3. Initialize EGL
    EGLint major, minor;
    if (!eglInitialize(eglDpy, &major, &minor)) {
        std::cout << "[FATAL] eglInitialize failed!" << std::endl;
        std::cout << "Reason: " << GetEGLErrorString(eglGetError()) << std::endl;
        return false;
    }
    
    std::cout << "EGL Initialized Successfully! Version: " << major << "." << minor << std::endl;

    // 4. Configure the invisible canvas
    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    
    EGLint numConfigs;
    EGLConfig eglCfg;
    if (!eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs)) {
        std::cout << "[FATAL] eglChooseConfig failed!" << std::endl;
        std::cout << "Reason: " << GetEGLErrorString(eglGetError()) << std::endl;
        return false;
    }

    // 5. Create PBuffer
    EGLint pbufferAttribs[] = {
        EGL_WIDTH, width,
        EGL_HEIGHT, height,
        EGL_NONE,
    };
    EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, pbufferAttribs);
    if (eglSurf == EGL_NO_SURFACE) {
        std::cout << "[FATAL] eglCreatePbufferSurface failed!" << std::endl;
        std::cout << "Reason: " << GetEGLErrorString(eglGetError()) << std::endl;
        return false;
    }

    // 6. Bind API & Create Context
    eglBindAPI(EGL_OPENGL_API);
    EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, NULL);
    if (eglCtx == EGL_NO_CONTEXT) {
        std::cout << "[FATAL] eglCreateContext failed!" << std::endl;
        std::cout << "Reason: " << GetEGLErrorString(eglGetError()) << std::endl;
        return false;
    }

    // 7. Make it Current
    if (!eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx)) {
        std::cout << "[FATAL] eglMakeCurrent failed!" << std::endl;
        std::cout << "Reason: " << GetEGLErrorString(eglGetError()) << std::endl;
        return false;
    }

    return true;
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
