#include "headers/main.hpp"
#include "headers/shapes.hpp"


int main()
{
    InitializeGLFW(4, 5);
    CreateWindow(800, 600, "Projctiles");
    InitializeGLEW();


    std::vector<float> sphereVertices = GenerateSphereVertices();
    std::vector<unsigned int> sphereIndices = GenerateSphereIndices();

    
}
