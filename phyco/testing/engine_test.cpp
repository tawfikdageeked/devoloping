#include "/devoloping/phyco/src_for_phyco/Phyco.hpp"

int main()
{
    Phyco.Start("main", 800, 600, "Phyco Test");
    ShaderManager.UseDefaultShader();

    Triangle tri(0.5);

    tri.SetColor(1.0f, 0.0f, 0.0f);
    Phyco.Wait(4, tri);

    tri.SetColor(0.5f, 0.6f, 0.8f);
    Phyco.Wait(4, tri);

    tri.SetColor(0.5f, 0.6f, 0.7f);
    Phyco.Wait(4, tri);

    return 0;
}