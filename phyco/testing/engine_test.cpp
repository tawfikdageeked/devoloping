#include "/devoloping/phyco/src_for_phyco/Phyco.hpp"
#include <cmath>

int main()
{
    Phyco.Start("main", 1200, 800, "The 3D Universe Sandbox");

    Phyco.SetCamera(0.0f, 5.0f, 12.0f);

    Plane floor;
    floor.SetPosition(0.0f, -2.0f, 0.0f);
    floor.SetColor(Colors::DarkGray);
    Phyco.Add(floor);

    Sphere sun(1.0f, 32, 32);
    sun.SetPosition(0.0f, 0.0f, 0.0f);
    sun.SetColor(Colors::Yellow);
    Phyco.Add(sun);

    Torus ring(2.0f, 0.15f, 32, 32);
    ring.SetPosition(0.0f, 0.0f, 0.0f);
    ring.SetColor(Colors::Orange);
    Phyco.Add(ring);

    Dodecahedron planet(0.6f);
    planet.SetColor(Colors::Cyan);
    Phyco.Add(planet);

    Cube moon(0.25f);
    moon.SetColor(Colors::White);
    Phyco.Add(moon);

    Pyramid pyra(0.8f);
    pyra.SetColor(Colors::Magenta);
    pyra.SetPosition(-5.0f, 0.0f, 0.0f);
    Phyco.Add(pyra);

    Cylinder pillar(2.0f, 0.5f);
    pillar.SetPosition(5.0f, -1.0f, 0.0f);
    pillar.SetColor(Colors::Green);
    Phyco.Add(pillar);

    float time = 0.0f;

    sun.AddUpdater([&](float dt) {
        time += dt;
        sun.scale = 1.0f + sin(time * 2.0f) * 0.05f; 
    });

    ring.AddUpdater([&](float dt) {
        ring.y = sin(time) * 0.5f;
    });

    planet.AddUpdater([&](float dt) {
        planet.x = cos(time) * 4.5f;
        planet.z = sin(time) * 4.5f;
    });

    moon.AddUpdater([&](float dt) {
        moon.x = planet.x + cos(time * 4.0f) * 1.5f;
        moon.z = planet.z + sin(time * 4.0f) * 1.5f;
        moon.y = planet.y + sin(time * 5.0f) * 0.5f; 
    });

    pyra.AddUpdater([&](float dt) {
        pyra.y = abs(sin(time * 3.0f)) * 2.0f - 1.0f; 
    });

    Phyco.Run();

    return 0;
}
