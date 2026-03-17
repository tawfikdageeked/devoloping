#include "/devoloping/phyco/src_for_phyco/Phyco.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.TakeHints();
    WindowManager.CreateWindow("main", 800, 600, "Phyco");
    WindowManager.UseWindow("main");

    ShaderManager.UseDefaultShader();

    Triangle tri(0.5);

    tri.SetColor(1.0f, 0.0f, 0.0f);
    Phyco.Add(tri);
    Phyco.Wait(4, "main");

    tri.SetColor(0.5f, 0.6f, 0.8f);
    Phyco.Wait(4, "main");

    tri.SetColor(0.95f, 0.3f, 0.3f);
    Phyco.Wait(4, "main");

    return 0;
}