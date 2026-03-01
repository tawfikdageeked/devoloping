#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

struct Body {
    double x, y, vx, vy, mass;
    sf::Color color;
    float radius;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(720, 480), "The CPU Melter (N-Body)");
    window.setFramerateLimit(60);

    sf::RenderTexture trailCanvas;
    trailCanvas.create(720, 480);
    trailCanvas.clear(sf::Color::Black);

    const double G = 6.6743e-11;
    
    // THE TORTURE DIALS
    double dt = 500;           
    int steps_per_frame = 20; 
    int num_asteroids = 100;  // Try changing this to 800 if it survives

    // The Core Planets (Added Jupiter to mess with the asteroids)
    std::vector<Body> system = {
        {0, 0, 0, 0, 1.989e30, sf::Color::Yellow, 10.f},             
        {5.79e10, 0, 0, 47360, 3.301e23, sf::Color(169,169,169), 2.f}, 
        {1.082e11, 0, 0, 35020, 4.867e24, sf::Color(255,204,153), 4.f},
        {1.496e11, 0, 0, 29780, 5.972e24, sf::Color::Cyan, 4.f},      
        {2.279e11, 0, 0, 24070, 6.39e23, sf::Color::Red, 3.f},         
        {7.785e11, 0, 0, 13070, 1.898e27, sf::Color(200,150,100), 8.f} 
    };

    // PROCEDURAL GENERATION: Injecting the Asteroid Belt
    srand(42); 
    for (int i = 0; i < num_asteroids; i++) {
        // Randomize angle and distance (between Mars and Jupiter)
        double angle = (rand() % 360) * 3.14159 / 180.0;
        double distance = 3.5e11 + (rand() % 250) * 1e9; 
        
        double x = distance * cos(angle);
        double y = distance * sin(angle);
        
        // Give them perfect circular velocity to start
        double v_circ = sqrt((G * 1.989e30) / distance);
        double vx = -v_circ * sin(angle);
        double vy = v_circ * cos(angle);
        
        // Push them into your math engine
        system.push_back({x, y, vx, vy, 1e15, sf::Color(150, 150, 150, 80), 1.0f});
    }

    sf::CircleShape shape;
    sf::CircleShape trailDot(0.5f);
    double scale = 2.5e9; // Zoomed way out to see Jupiter

    // THE ACCURACY UPGRADE
    double softening = 5e8; // Prevents division by zero errors

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // --- THE MEAT GRINDER ---
        for (int step = 0; step < steps_per_frame; step++) {
            std::vector<double> ax(system.size(), 0.0);
            std::vector<double> ay(system.size(), 0.0);

            for (size_t i = 0; i < system.size(); i++) {
                for (size_t j = 0; j < system.size(); j++) {
                    if (i == j) continue;

                    double dx = system[j].x - system[i].x;
                    double dy = system[j].y - system[i].y;
                    
                    // The Softened Gravity Math
                    double r2 = dx*dx + dy*dy + softening*softening;
                    double r = sqrt(r2);
                    
                    double accel = (G * system[j].mass) / (r2 * r);
                    ax[i] += accel * dx;
                    ay[i] += accel * dy;
                }
            }

            for (size_t i = 0; i < system.size(); i++) {
                system[i].vx += ax[i] * dt;
                system[i].vy += ay[i] * dt;
                system[i].x += system[i].vx * dt;
                system[i].y += system[i].vy * dt;

                // Only draw trails for the main planets, not the 400 asteroids
                if (i > 0 && i < 6) {
                    trailDot.setFillColor(system[i].color);
                    trailDot.setPosition(system[i].x / scale + 360, system[i].y / scale + 240);
                    trailCanvas.draw(trailDot);
                }
            }
        }
        trailCanvas.display();

        window.clear();
        sf::Sprite trailSprite(trailCanvas.getTexture());
        window.draw(trailSprite);

        for (const auto& b : system) {
            shape.setRadius(b.radius);
            shape.setFillColor(b.color);
            shape.setOrigin(b.radius, b.radius);
            shape.setPosition(b.x / scale + 360, b.y / scale + 240);
            window.draw(shape);
        }
        window.display();
        
        static int frame = 0;
        if (frame++ % 60 == 0) {
            std::cout << "Crunching " << system.size() << " bodies..." << std::endl;
        }
    }
    return 0;
}