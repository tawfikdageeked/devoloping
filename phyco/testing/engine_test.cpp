#include "/devoloping/phyco/src_for_phyco/Phyco.hpp"

int main()
{
    Phyco.Start("main", 1200, 800, "The 3D Engine");
    ShaderManager.UseDefaultShader();

    Phyco.SetCamera(0.0f, 0.0f, 5.0f); 

   

    Cube myCube(1.0f);
    myCube.SetPosition(0.0f, 0.0f, 0.0f);
    myCube.SetColor(Colors::Cyan);
    Phyco.Add(myCube);

    myCube.AddUpdater([&](float dt) {
        myCube.x += 0.5f * dt;
        if (myCube.x > 2.0f) myCube.x = -2.0f;
    });

    Phyco.Run();

    return 0;
}
