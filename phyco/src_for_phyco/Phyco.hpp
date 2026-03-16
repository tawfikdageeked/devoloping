#ifndef PHYCO_HPP
#define PHYCO_HPP

#include "/devoloping/phyco/src_for_phyco/Objects.hpp"
#include "/devoloping/phyco/src_for_phyco/WindowManager.hpp"



class PHFW
{
    public:

    std::string activeWindow;

    void Start(std::string name, int width, int height, const char* title)
    {
        WindowManager.Start();
        WindowManager.CreateWindow(name, width, height, title);
        WindowManager.UseWindow(name);
        activeWindow = name;
    }

    
    void Wait(float seconds, Shape& shape, std::string name)
    {
        double start = glfwGetTime();

        while (glfwGetTime() - start < seconds)
        {
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shape.Draw();

            WindowManager.Update(activeWindow);
            WindowManager.Manage();

        }
    }


};

PHFW Phyco;

#endif