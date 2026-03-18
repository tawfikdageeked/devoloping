#ifndef PHYCO_HPP
#define PHYCO_HPP

#include "/devoloping/phyco/src_for_phyco/Objects.hpp"
#include "/devoloping/phyco/src_for_phyco/WindowManager.hpp"
#include <vector>



class PHFW
{
    public:
    float dt;
    std::string activeWindow;
    std::vector<Shape2D*> Shapes2DOnScreen;
    std::vector<Shape3D*> Shapes3DOnScreen;

    int winWidth, winHeight;
    
    glm::vec3 cameraPos   = glm::vec3(0.0f, 1.0f, 4.0f); 
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); 
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    void Start(std::string name, int width, int height, const char* title)
    {
        winWidth = width;
        winHeight = height;

        WindowManager.Start();
        WindowManager.TakeHints();
        WindowManager.CreateWindow(name, width, height, title);
        WindowManager.UseWindow(name);    
        
        activeWindow = name;

        glEnable(GL_DEPTH_TEST);

        WindowManager.Update(activeWindow);
        WindowManager.Manage();

        std::cout << "Attention!: Default Parameters Are Used By Phyco.Start()\n";
    }

    void Add(Shape2D& shape)
    {
       Shapes2DOnScreen.push_back(&shape);
    }

    void Add(Shape3D& shape)
    {
       Shapes3DOnScreen.push_back(&shape);
    }

    void SetCamera(float x, float y, float z)
    {
        cameraPos = glm::vec3(x, y, z);
    }

    void Run()
    {
        double lastframe = glfwGetTime();

        while (WindowManager.IsOpen(activeWindow))
        {
            double currentframe = glfwGetTime();
            dt = currentframe - lastframe;
            lastframe = currentframe;

            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);

            ShaderManager.SetMat4("DefaultShader", "uView", view);
            ShaderManager.SetMat4("DefaultShader", "uProjection", projection);

            for(Shape2D* s : Shapes2DOnScreen)
            {
                s->UpdateLogic(dt);
            }

            for(Shape2D* s: Shapes2DOnScreen)
            {
                s->Draw();
            }

            for(Shape3D* s : Shapes3DOnScreen)
            {
                s->UpdateLogic(dt);
            }

            for(Shape3D* s: Shapes3DOnScreen)
            {
                s->Draw();
            }

            WindowManager.Update(activeWindow);
            WindowManager.Manage();
        }
    }
};

PHFW Phyco;

#endif