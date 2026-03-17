#include "/devoloping/phyco/src_for_phyco/Phyco.hpp"
#include <iostream>

int main()
{
    Phyco.Start("main", 800, 600, "Phyco Test");
    ShaderManager.UseDefaultShader();

    Triangle tri(0.5);
    tri.SetColor(1.0f, 0.0f, 0.0f);
    tri.SetPosition(-0.5f, -0.6f, 0.0f);
    
    Phyco.Add(tri);

    tri.AddUpdater([&](float dt) {
        tri.x += 0.01f * dt;
        tri.y += 0.01f * dt;
    });
    

    Phyco.Wait(4, "main");

    return 0;
}