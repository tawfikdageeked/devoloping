

/*
first we need to initialize GLFW with : glfwInit();
GLFW doesn't know what verison of opengl we are going to use so it need to know by literally giving it a hint:

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

opengl goes way back so it have sort of two shapes modern one and old one
the modern one is called to use the modern one only you use the core profile and to use both old and new one you use compatability

glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)
now creating the window itself

GLFWwindow* window = glfwCreateWindow(width, height, name of the window, fullscreen or not, NOT IMPORTANT)

*/

#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <string>



std::map<std::string, GLFWwindow*> windows;

class WindowMngr
{
    public:
    
    // apparently all those words are just certain integers so i made those variables for simplicity
    // makes me wonder why they didn't make it simpler in the first place
    static const int prof = GLFW_OPENGL_PROFILE;
    static const int core = GLFW_OPENGL_CORE_PROFILE;
    static const int comp = GLFW_OPENGL_COMPAT_PROFILE;
    static const int major_ver_hint = GLFW_CONTEXT_VERSION_MAJOR;
    static const int minor_ver_hint = GLFW_CONTEXT_VERSION_MINOR;
    
    // start method it initialize the the WindowManager and have some default values for the Hints
    void Start()
    {
        glfwInit();
        glfwWindowHint(major_ver_hint, 3);
        glfwWindowHint(minor_ver_hint, 3);
        glfwWindowHint(prof, core);
    }
    // TakeHints method it overrides the default hint values to what ever the user want
    int TakeHints(int major_ver, int minor_ver, int profile)
    {
        if(profile != comp && profile != core)
        {
            std::cout << "That's not a defined profile, Please use a defined profile\n";
            return -1;
        }
        glfwWindowHint(major_ver_hint, major_ver);
        glfwWindowHint(minor_ver_hint, minor_ver);
        glfwWindowHint(prof, profile);
        return 0;
    }


    // create window responsible for creating the windows with whatever parameters you want

    // first parameter is sort of a pointer to the window so the user doesn't have to interact with GLFW as possible he can use that pointer
    // second is width of the window, third is height of the window third it he title of the window

    GLFWwindow* CreateWindow(std::string name, int width, int height, const char* title)
    {
        windows[name] = glfwCreateWindow(width, height, title, NULL, NULL);
        
        if (windows[name] == NULL)
        {
            std::cout << "Failed to create window\n";
            glfwTerminate();
            return nullptr;
        }
        return windows[name];
    }

    // UseWindow methon is resposible for telling opengl use this window it links the window the graphic api and intializing GLEW
    void UseWindow(std::string name)
    {
        glfwMakeContextCurrent(windows[name]);

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        if(err != GLEW_OK)
        {
            std::cout << "GLEW Error: " << glewGetErrorString(err) << '\n';
        }
        else
        {
            std::cout << "GLEW OK\n";
            std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
        }
    }

    // IsOpen method checks if the window is opened
    bool IsOpen(std::string name)
    {
        return !glfwWindowShouldClose(windows[name]);
    }

    // Update method updates the frames of the window
    void Update(std::string name)
    {
        glfwSwapBuffers(windows[name]);
    }
    

    // Manage methond resposible for making the window resizable and take inputs like mouse clicks or keyboard presses
    void Manage()
    {
        glfwPollEvents();
    }

    // StopWindow method kills windowssrc_for_engine
    void StopWindow(std::string name)
    {
        glfwDestroyWindow(windows[name]);
    }

    // Stop method responsible for killing the window manger itself
    void Stop()
    {
        glfwTerminate();
    }

};

WindowMngr WindowManager;

#endif