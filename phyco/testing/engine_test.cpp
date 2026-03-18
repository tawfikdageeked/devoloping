#include "/devoloping/phyco/src_for_phyco/Phyco.hpp"
#include <cmath>

int main()
{
    // 1. Initialize the Engine
    Phyco.Start("main", 1200, 800, "The 3D Tumbling Universe");
    
    // 2. CRITICAL: Compile the 3D Shaders so the GPU knows how to draw light and shadow!
    ShaderManager.UseDefaultShader(); 

    // 3. Set the Camera back and up so we can see the whole solar system
    Phyco.SetCamera(0.0f, 6.0f, 14.0f);

    // --- SPAWN THE UNIVERSE ---
    
    Plane floor;
    floor.SetPosition(0.0f, -2.0f, 0.0f);
    floor.SetColor(Colors::DarkGray);
    Phyco.Add(floor);

    Sphere sun(1.0f, 32, 32);
    sun.SetPosition(0.0f, 0.0f, 0.0f);
    sun.SetColor(Colors::Yellow);
    Phyco.Add(sun);

    Torus ring(2.5f, 0.15f, 32, 32);
    ring.SetPosition(0.0f, 0.0f, 0.0f);
    ring.SetColor(Colors::Orange);
    // Tilt the ring slightly so it looks cool from the start
    ring.rx = 15.0f; 
    Phyco.Add(ring);

    Dodecahedron planet(0.6f);
    planet.SetColor(Colors::Cyan);
    Phyco.Add(planet);

    Cube moon(0.25f);
    moon.SetColor(Colors::White);
    Phyco.Add(moon);

    Pyramid pyra(0.8f);
    pyra.SetColor(Colors::Magenta);
    pyra.SetPosition(-6.0f, 0.0f, 0.0f);
    Phyco.Add(pyra);

    Cylinder pillar(2.0f, 0.5f);
    pillar.SetPosition(6.0f, -1.0f, 0.0f);
    pillar.SetColor(Colors::Green);
    Phyco.Add(pillar);

    // --- ADD THE PHYSICS AND MATH UPDATERS ---
    
    float time = 0.0f;

    sun.AddUpdater([&](float dt) {
        time += dt;
        sun.scale = 1.0f + sin(time * 2.0f) * 0.05f; // Pulse like a star
        sun.ry += 15.0f * dt; // Spin slowly
    });

    ring.AddUpdater([&](float dt) {
        ring.y = sin(time) * 0.5f; // Bob up and down
        ring.ry -= 30.0f * dt;     // Spin opposite to the sun
    });

    planet.AddUpdater([&](float dt) {
        // Orbit around the sun using Sine and Cosine
        planet.x = cos(time) * 5.0f;
        planet.z = sin(time) * 5.0f;
        
        // NEW: Spin the planet on its own Y axis!
        planet.ry += 60.0f * dt; 
    });

    moon.AddUpdater([&](float dt) {
        // Orbit the moving planet
        moon.x = planet.x + cos(time * 4.0f) * 1.5f;
        moon.z = planet.z + sin(time * 4.0f) * 1.5f;
        moon.y = planet.y + sin(time * 5.0f) * 0.5f; 
        
        // NEW: Make the moon tumble uncontrollably in all 3 dimensions!
        moon.rx += 90.0f * dt;
        moon.ry += 45.0f * dt;
        moon.rz += 120.0f * dt;
    });

    pyra.AddUpdater([&](float dt) {
        // Bounce on the floor
        pyra.y = abs(sin(time * 3.0f)) * 2.0f - 1.0f; 
        
        // NEW: Spin the pyramid like a drill bit
        pyra.ry -= 150.0f * dt; 
    });

    pillar.AddUpdater([&](float dt) {
        // Gently sway the cylinder back and forth to show off the curved lighting
        pillar.rx = sin(time) * 20.0f;
        pillar.rz = cos(time) * 20.0f;
    });

    // 4. Ignite the Engine
    Phyco.Run();

    return 0;
}
