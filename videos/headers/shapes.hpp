#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <array>


/* vertices format

[x, y, z, r, g, b, a, nx, ny, nz, u, v]

*/


const float PI = 3.14159265359f;


std::vector<float> GenerateSphereVertices(float radius = 0.5f, int sectors= 20, int stacks = 15, std::array<float, 4> color = {1.0f, 0.0f, 0.0f, 0.0f}) {
    std::vector<float> vertices;

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2 - i * stackStep; // From pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);      // r * cos(phi)
        float z = radius * sinf(stackAngle);       // r * sin(phi)

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;    // From 0 to 2pi

            // 1. Positions
            float x = xy * cosf(sectorAngle);      // r * cos(phi) * cos(theta)
            float y = xy * sinf(sectorAngle);      // r * cos(phi) * sin(theta)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // 2. Colors
            vertices.push_back(color[0]);
            vertices.push_back(color[1]);
            vertices.push_back(color[2]);
            vertices.push_back(color[3]);

            // 3. Normals (For a sphere, normal is just position / radius)
            vertices.push_back(x / radius);
            vertices.push_back(y / radius);
            vertices.push_back(z / radius);

            // 4. Texture Coordinates (UVs)
            float s = (float)j / sectors;
            float t = (float)i / stacks;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }
    
    return vertices;
}


std::vector<unsigned int> GenerateSphereIndices(int sectors = 20, int stacks = 15) {
    std::vector<unsigned int> indices;

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);     // Beginning of current stack
        int k2 = k1 + sectors + 1;      // Beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // 2 triangles per sector (excluding the top and bottom poles)
            
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return indices;
}



// resX and resZ control how many grid squares make up the plane.
// For a simple flat floor, resX=1 and resZ=1 is enough (just 4 vertices).
std::vector<float> GeneratePlaneVertices(float width = 1, float depth = 1, int resX = 20, int resZ = 20, std::array<float, 4> color = {1.0f, 1.0f, 0.0f, 0.0f}) {
    std::vector<float> vertices;

    for (int z = 0; z <= resZ; ++z) {
        for (int x = 0; x <= resX; ++x) {
            
            // 1. Positions (Centered at 0,0,0 on the XZ plane)
            float posX = ((float)x / resX - 0.5f) * width;
            float posY = 0.0f; 
            float posZ = ((float)z / resZ - 0.5f) * depth;

            vertices.push_back(posX);
            vertices.push_back(posY);
            vertices.push_back(posZ);

            // 2. Colors
            vertices.push_back(color[0]);
            vertices.push_back(color[1]);
            vertices.push_back(color[2]);
            vertices.push_back(color[3]);

            
            // 3. Normals (Always pointing straight up for a flat floor)
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            

            // 4. UVs (Stretching from 0.0 to 1.0 across the whole plane)
            float u = (float)x / resX;
            float v = (float)z / resZ;
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }
    return vertices;
}

std::vector<unsigned int> GeneratePlaneIndices(int resX = 20, int resZ = 20) {
    std::vector<unsigned int> indices;

    for (int z = 0; z < resZ; ++z) {
        for (int x = 0; x < resX; ++x) {
            
            // Calculate the 4 corners of the current grid square
            int topLeft     = z * (resX + 1) + x;
            int topRight    = topLeft + 1;
            int bottomLeft  = (z + 1) * (resX + 1) + x;
            int bottomRight = bottomLeft + 1;

            // Triangle 1 (Top-Left, Bottom-Left, Top-Right)
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triangle 2 (Top-Right, Bottom-Left, Bottom-Right)
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    return indices;
}
