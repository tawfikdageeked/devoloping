#include "/devoloping/phyco/src_for_phyco/WindowManager.hpp"
#include "/devoloping/phyco/src_for_phyco/ShaderManager.hpp"
#include "/devoloping/phyco/src_for_phyco/Objects.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.CreateWindow("main", 800, 600, "Phyco Engine");
    WindowManager.UseWindow("main");

    const char* vert = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}\n";

    const char* frag = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(1.0, 0.3, 0.2, 1.0);\n"
        "}\n";

    ShaderManager.CreateShaderFromString("basic", vert, frag);

    Triangle tri(0.5, 1.0, 0.0, 0.0);

    while (WindowManager.IsOpen("main"))
    {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ShaderManager.UseProgram("basic");

        std::cout << "VAO: " << tri.VAO << '\n';

        tri.Draw();

        WindowManager.Update("main");
        WindowManager.Manage();
    }

    tri.Delete();
    ShaderManager.DeleteAllShader();
    WindowManager.StopWindow("main");
    WindowManager.Stop();
    return 0;
}