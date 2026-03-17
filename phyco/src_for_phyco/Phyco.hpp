#ifndef PHYCO_HPP
#define PHYCO_HPP

#include "/devoloping/phyco/src_for_phyco/Objects.hpp"
#include "/devoloping/phyco/src_for_phyco/WindowManager.hpp"
#include <vector>



class PHFW
{
    public:

    std::vector<Shape*> ShapesOnScreen;


    void Start(std::string name, int width, int height, const char* title)
    {
        WindowManager.Start();
        WindowManager.TakeHints();
        WindowManager.CreateWindow(name, width, height, title);
        WindowManager.UseWindow(name);    
        WindowManager.Update(name);
        WindowManager.Manage();
    
        std::cout << "Attention!: Default Parameters Are Used By Phyco.Start()\n";
    }

    void Add(Shape& shape)
    {
       ShapesOnScreen.push_back(&shape);
    }
    
    void Wait(float seconds, std::string name)
    {
        double start = glfwGetTime();

        while (glfwGetTime() - start < seconds)
        {
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for(Shape* s: ShapesOnScreen)
            {
            s -> Draw();
            }

            WindowManager.Update(name);
            WindowManager.Manage();

        }
    }


};

PHFW Phyco;

#endif