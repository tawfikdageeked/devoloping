#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <GL/glew.h>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <functional>
#include "ShaderManager.hpp"
#include "Colors.hpp"

class Shape2D
{   
    public:
    unsigned int VAO;
    unsigned int VBO;
    int vertexcount;
    float r, g, b, opacity;
    float x, y, z;
    float scale = 1.0f;

    std::string shaderName = "DefaultShader";
    unsigned int drawMode = GL_TRIANGLES; 

    std::vector<std::function<void(float)>> updaters;

    void AddUpdater(std::function<void(float)> updater)
    {
        updaters.push_back(updater);
    }

    void UpdateLogic(float dt)
    {
        for(std::function<void(float)>& func : updaters)
        {
            func(dt);
        }
    }

    void SetColor(const float color[3], float alpha = 1.0f)
    {
        r = color[0]; g = color[1]; b = color[2]; opacity = alpha;
    }

    void SetColor(float red, float green, float blue, float alpha = 1.0f)
    {
        r = red; g = green; b = blue; opacity = alpha;
    }

    void SetPosition(float x_cor, float y_cor, float z_cor)
    {
        x = x_cor; y = y_cor; z = z_cor;
    }

    void SetScale(float s)
    {
        scale = s;
    }

    void Upload(float* vertices, int totalfloats)
    {
        vertexcount = totalfloats / 3;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, totalfloats * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void UpdateVertices(float* new_vertices, int totalfloats)
    {
        vertexcount = totalfloats / 3;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, totalfloats * sizeof(float), new_vertices, GL_DYNAMIC_DRAW);
        glBindVertexArray(0);
    }

    void Draw()
    {
        ShaderManager.UseProgram(shaderName);
        ShaderManager.SetVec3(shaderName, "uColor", r, g, b);
        ShaderManager.SetVec3(shaderName, "uOffset", x, y, z);
        ShaderManager.SetFloat(shaderName, "uScale", scale);

        glBindVertexArray(VAO);
        glDrawArrays(drawMode, 0, vertexcount);
    }

    void Delete()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

// 1. The Classic Triangle
class Triangle : public Shape2D
{
    public:
    float sizeLength;

    Triangle(float size)
    {
        sizeLength = size;
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        float h = (sqrt(3.0f)/(2.0f) * size);
        float vertices[] = {
            0.0f, 2.0f * h / 3.0f, 0.0f,
            -size / 2, -h / 3.0f, 0.0f,
            size / 2, -h / 3.0f, 0.0f
        };
        Upload(vertices, 9);
    }
};

// 2. The Physics Floor / Box
class Rectangle : public Shape2D
{
    public:
    Rectangle(float w, float h)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        float vertices[] = {
            -w/2,  h/2, 0.0f, 
            -w/2, -h/2, 0.0f, 
             w/2, -h/2, 0.0f, 
            -w/2,  h/2, 0.0f, 
             w/2, -h/2, 0.0f, 
             w/2,  h/2, 0.0f  
        };
        Upload(vertices, 18);
    }
};

// 3. The Dynamic Line (Vectors, Rays, Strings)
class Line : public Shape2D
{
    public:
    Line(float startX, float startY, float endX, float endY)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_LINES; 

        float vertices[] = {
            startX, startY, 0.0f,
            endX, endY, 0.0f
        };
        Upload(vertices, 6);
    }
};

// 4. The Particle (Single Pixel / Dot)
class Point : public Shape2D
{
    public:
    Point()
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_POINTS; 

        float vertices[] = { 0.0f, 0.0f, 0.0f };
        Upload(vertices, 3);
    }
};

// 5. The Perfect Circle (Trigonometry generated)
class Circle : public Shape2D
{
    public:
    Circle(float radius, int segments = 50)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        float angleStep = (2.0f * 3.14159265f) / segments;

        for (int i = 0; i < segments; i++)
        {
            float currentAngle = i * angleStep;
            float nextAngle = (i + 1) * angleStep;

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(radius * cos(currentAngle));
            vertices.push_back(radius * sin(currentAngle));
            vertices.push_back(0.0f);

            vertices.push_back(radius * cos(nextAngle));
            vertices.push_back(radius * sin(nextAngle));
            vertices.push_back(0.0f);
        }
        Upload(vertices.data(), vertices.size());
    }
};

// 6. Regular Polygon (Hexagon, Octagon, etc. using GL_LINE_LOOP for a wireframe look)
class RegularPolygon : public Shape2D
{
    public:
    RegularPolygon(float radius, int sides)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_LINE_LOOP; 

        std::vector<float> vertices;
        float angleStep = (2.0f * 3.14159265f) / sides;

        for (int i = 0; i < sides; i++)
        {
            float angle = i * angleStep;
            vertices.push_back(radius * cos(angle));
            vertices.push_back(radius * sin(angle));
            vertices.push_back(0.0f);
        }
        Upload(vertices.data(), vertices.size());
    }
};

// 7. The Math Star (Alternating inner and outer radius)
class Star : public Shape2D
{
    public:
    Star(float innerRadius, float outerRadius, int points)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        int totalSegments = points * 2;
        float angleStep = (2.0f * 3.14159265f) / totalSegments;

        for (int i = 0; i < totalSegments; i++)
        {
            float r1 = (i % 2 == 0) ? outerRadius : innerRadius;
            float r2 = ((i + 1) % 2 == 0) ? outerRadius : innerRadius;

            float currentAngle = i * angleStep;
            float nextAngle = (i + 1) * angleStep;

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(r1 * cos(currentAngle));
            vertices.push_back(r1 * sin(currentAngle));
            vertices.push_back(0.0f);

            vertices.push_back(r2 * cos(nextAngle));
            vertices.push_back(r2 * sin(nextAngle));
            vertices.push_back(0.0f);
        }
        Upload(vertices.data(), vertices.size());
    }
};



// The math constant PI, which we will use heavily for spheres, cones, and cylinders
const float PI = 3.14159265359f;

class Shape3D
{   
    public:
    unsigned int VAO, VBO, EBO; // Reforged to use an Element Buffer Object for indexing
    int indexCount; // The EBO tells us how many total triangles to draw, not glDrawArrays(0, vertexCount)
    float r, g, b, opacity;
    float x, y, z;
    float scale = 1.0f;

    std::string shaderName = "DefaultShader";
    unsigned int drawMode = GL_TRIANGLES; 

    std::vector<std::function<void(float)>> updaters;

    void AddUpdater(std::function<void(float)> updater)
    {
        updaters.push_back(updater);
    }

    void UpdateLogic(float dt)
    {
        for(std::function<void(float)>& func : updaters)
        {
            func(dt);
        }
    }

    void SetColor(const float color[3], float alpha = 1.0f)
    {
        r = color[0]; g = color[1]; b = color[2]; opacity = alpha;
    }

    void SetColor(float red, float green, float blue, float alpha = 1.0f)
    {
        r = red; g = green; b = blue; opacity = alpha;
    }

    void SetPosition(float x_cor, float y_cor, float z_cor)
    {
        x = x_cor; y = y_cor; z = z_cor;
    }

    void SetScale(float s)
    {
        scale = s;
    }

    // Reforged: Upload now takes a data vector that includes both Positions and Normals, and an Indices vector
    void Upload(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    {
        indexCount = indices.size();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO); // Generate the new Index Buffer

        glBindVertexArray(VAO);

        // Upload combined position + normal data to the VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Upload index data to the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex Attribute 0: Position (x, y, z). Size 3, Stride 6 (3 pos + 3 normal)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex Attribute 1: Normal Vector (nx, ny, nz). Size 3, Stride 6, starts at offset 3
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void Draw()
    {
        ShaderManager.UseProgram(shaderName);
        ShaderManager.SetVec3(shaderName, "uColor", r, g, b);
        ShaderManager.SetVec3(shaderName, "uOffset", x, y, z);
        ShaderManager.SetFloat(shaderName, "uScale", scale);

        glBindVertexArray(VAO);
        
        // Reforged: Use glDrawElements to draw using the Index Buffer
        glDrawElements(drawMode, indexCount, GL_UNSIGNED_INT, 0); 
    }

    void Delete()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

// 1. The Perfect 3D Cube (A "unit" cube, 1x1x1, centered at origin)
class Cube : public Shape3D
{
    public:
    Cube(float size = 1.0f)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = size;
        drawMode = GL_TRIANGLES;

        float h = 0.5f;
        // Vertex Data format: Position (x,y,z), Normal (nx,ny,nz)
        std::vector<float> vertices = {
            // Back face
            -h, -h, -h,  0.0f,  0.0f, -1.0f,
             h, -h, -h,  0.0f,  0.0f, -1.0f,
             h,  h, -h,  0.0f,  0.0f, -1.0f,
            -h,  h, -h,  0.0f,  0.0f, -1.0f,
            // Front face
            -h, -h,  h,  0.0f,  0.0f,  1.0f,
             h, -h,  h,  0.0f,  0.0f,  1.0f,
             h,  h,  h,  0.0f,  0.0f,  1.0f,
            -h,  h,  h,  0.0f,  0.0f,  1.0f,
            // Left face
            -h,  h,  h, -1.0f,  0.0f,  0.0f,
            -h,  h, -h, -1.0f,  0.0f,  0.0f,
            -h, -h, -h, -1.0f,  0.0f,  0.0f,
            -h, -h,  h, -1.0f,  0.0f,  0.0f,
            // Right face
             h,  h,  h,  1.0f,  0.0f,  0.0f,
             h,  h, -h,  1.0f,  0.0f,  0.0f,
             h, -h, -h,  1.0f,  0.0f,  0.0f,
             h, -h,  h,  1.0f,  0.0f,  0.0f,
            // Bottom face
            -h, -h, -h,  0.0f, -1.0f,  0.0f,
             h, -h, -h,  0.0f, -1.0f,  0.0f,
             h, -h,  h,  0.0f, -1.0f,  0.0f,
            -h, -h,  h,  0.0f, -1.0f,  0.0f,
            // Top face
            -h,  h, -h,  0.0f,  1.0f,  0.0f,
             h,  h, -h,  0.0f,  1.0f,  0.0f,
             h,  h,  h,  0.0f,  1.0f,  0.0f,
            -h,  h,  h,  0.0f,  1.0f,  0.0f,
        };

        // Indices: The blueprint telling the GPU how to connect the 24 vertices into triangles
        std::vector<unsigned int> indices = {
            0, 2, 1, 0, 3, 2,       // Back face
            4, 5, 6, 4, 6, 7,       // Front face
            8, 10, 9, 8, 11, 10,    // Left face
            12, 13, 14, 12, 14, 15, // Right face
            16, 17, 18, 16, 18, 19, // Bottom face
            20, 22, 21, 20, 23, 22  // Top face
        };

        Upload(vertices, indices);
    }
};

// 2. The UV Sphere (Uses polar coordinates math to wrap a 2D grid into a sphere)
class Sphere : public Shape3D
{
    public:
    Sphere(float radius = 0.5f, int slices = 32, int stacks = 32)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = radius;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = PI / stacks * i; // angle for the 'stacks' (up-down)
            float yPos = cos(stackAngle); // Y coordinate (the radius from the axis)
            float sliceRadius = sin(stackAngle); // Radius of the circle at this stack height

            for (int j = 0; j <= slices; ++j) {
                float sliceAngle = 2 * PI / slices * j; // angle for the 'slices' (around the Z-axis)
                float xPos = sliceRadius * sin(sliceAngle); // X coordinate
                float zPos = sliceRadius * cos(sliceAngle); // Z coordinate

                // Sphere has an unique property: a vertex's position is also its normalized Normal vector!
                vertices.push_back(xPos); 
                vertices.push_back(yPos); 
                vertices.push_back(zPos); 
                vertices.push_back(xPos); // Normalnx
                vertices.push_back(yPos); // Normal ny
                vertices.push_back(zPos); // Normal nz
            }
        }

        // Generate indices, connecting the grid of vertices into triangles
        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                unsigned int first = (i * (slices + 1)) + j;
                unsigned int second = first + slices + 1;
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
        Upload(vertices, indices);
    }
};

// 3. The Cone (A circle base that tapers to a single point)
class Cone : public Shape3D
{
    public:
    Cone(float height = 1.0f, float radius = 0.5f, int segments = 32)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float h_half = height / 2.0f;
        float angleStep = 2 * PI / segments;

        // Base center point (index 0)
        vertices.push_back(0.0f); vertices.push_back(-h_half); vertices.push_back(0.0f);
        vertices.push_back(0.0f); vertices.push_back(-1.0f);   vertices.push_back(0.0f); // Base normal is just (0, -1, 0)

        // Generate base vertices (indices 1 to segments)
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float xPos = radius * cos(angle);
            float zPos = radius * sin(angle);
            vertices.push_back(xPos); vertices.push_back(-h_half); vertices.push_back(zPos);
            vertices.push_back(0.0f); vertices.push_back(-1.0f);   vertices.push_back(0.0f); // Base normal
        }

        // Generate indices for the circular base, forming a 'fan'
        for (int i = 1; i <= segments; ++i) {
            indices.push_back(0); indices.push_back(i); indices.push_back(i + 1);
        }

        // Add the top apex point (index segments + 2)
        vertices.push_back(0.0f); vertices.push_back(h_half); vertices.push_back(0.0f);
        vertices.push_back(0.0f); vertices.push_back(1.0f);   vertices.push_back(0.0f); // Apex normal is just (0, 1, 0)

        // Create separate vertices for the cone's curved surface to allow flat normals
        unsigned int offset = vertices.size() / 6; // current vertex index before the next step
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float xPos = radius * cos(angle);
            float zPos = radius * sin(angle);
            vertices.push_back(xPos); vertices.push_back(-h_half); vertices.push_back(zPos);
            vertices.push_back(cos(angle)); vertices.push_back(radius/height); vertices.push_back(sin(angle)); // Approximated 'slant' normal
        }

        // Generate indices for the slanted sides, connecting the apex to the side vertices
        unsigned int apexIndex = segments + 2;
        for (int i = 0; i < segments; ++i) {
            indices.push_back(apexIndex);
            indices.push_back(offset + i);
            indices.push_back(offset + i + 1);
        }
        Upload(vertices, indices);
    }
};

// 4. The Cylinder (Two circle bases connected by a curved pipe)
class Cylinder : public Shape3D
{
    public:
    Cylinder(float height = 1.0f, float radius = 0.5f, int segments = 32)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float h_half = height / 2.0f;
        float angleStep = 2 * PI / segments;

        // Bottom base center (index 0) and Top base center (index 1)
        vertices.push_back(0.0f); vertices.push_back(-h_half); vertices.push_back(0.0f);
        vertices.push_back(0.0f); vertices.push_back(-1.0f);   vertices.push_back(0.0f); // Bottom normal (0,-1,0)

        vertices.push_back(0.0f); vertices.push_back(h_half);  vertices.push_back(0.0f);
        vertices.push_back(0.0f); vertices.push_back(1.0f);    vertices.push_back(0.0f); // Top normal (0,1,0)

        // Generate Bottom and Top base vertices for the circular end-caps
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float xPos = radius * cos(angle);
            float zPos = radius * sin(angle);
            // Bottom vertex (index 2 to segments + 2)
            vertices.push_back(xPos); vertices.push_back(-h_half); vertices.push_back(zPos);
            vertices.push_back(0.0f); vertices.push_back(-1.0f);   vertices.push_back(0.0f); // Bottom normal (0,-1,0)

            // Top vertex (index segments + 3 to 2*segments + 3)
            vertices.push_back(xPos); vertices.push_back(h_half); vertices.push_back(zPos);
            vertices.push_back(0.0f); vertices.push_back(1.0f);   vertices.push_back(0.0f); // Top normal (0,1,0)
        }

        // Create Indices for the Bottom and Top caps, forming Fans
        for (int i = 0; i < segments; ++i) {
            indices.push_back(0);
            indices.push_back(2 + i);
            indices.push_back(2 + i + 1);

            indices.push_back(1);
            indices.push_back(segments + 3 + i + 1);
            indices.push_back(segments + 3 + i);
        }

        // Create unique vertices for the curved side wall to allow curved Normals
        unsigned int sideStart = vertices.size() / 6;
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float xPos = radius * cos(angle);
            float zPos = radius * sin(angle);
            
            // Side-Bottom vertex
            vertices.push_back(xPos); vertices.push_back(-h_half); vertices.push_back(zPos);
            vertices.push_back(cos(angle)); vertices.push_back(0.0f); vertices.push_back(sin(angle)); // Radial Normal vector

            // Side-Top vertex
            vertices.push_back(xPos); vertices.push_back(h_half); vertices.push_back(zPos);
            vertices.push_back(cos(angle)); vertices.push_back(0.0f); vertices.push_back(sin(angle)); // Radial Normal vector
        }

        // Generate indices for the side-wall 'quads', connecting Side-Bottom to Side-Top vertices
        for (int i = 0; i < segments; ++i) {
            unsigned int first = sideStart + i * 2;
            unsigned int second = first + 2;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
        Upload(vertices, indices);
    }
};

// 5. The Pyramid (A simple, low-poly 3-sided pyramid, known as a tetrahedron)
class Pyramid : public Shape3D
{
    public:
    Pyramid(float size = 1.0f)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = size;
        drawMode = GL_TRIANGLES;

        float h = 0.5f;
        std::vector<float> vertices = {
            // Base face (facing downwards)
            -h, -h, -h,  0.0f, -1.0f, 0.0f,
             h, -h, -h,  0.0f, -1.0f, 0.0f,
             0.0f, -h, h,  0.0f, -1.0f, 0.0f,
            // Back slant face (nx,ny,nz are approximated)
            -h, -h, -h,  -0.5f, 0.5f, -0.5f,
             h, -h, -h,   0.5f, 0.5f, -0.5f,
             0.0f, h, 0.0f, 0.0f, 1.0f, 0.0f, // Apex
            // Left slant face
            -h, -h, -h,  -0.5f, 0.5f, 0.5f,
             0.0f, -h, h,  0.0f, 0.5f, 0.5f,
             0.0f, h, 0.0f, 0.0f, 1.0f, 0.0f, // Apex
            // Right slant face
             h, -h, -h,   0.5f, 0.5f, 0.5f,
             0.0f, -h, h,  0.0f, 0.5f, 0.5f,
             0.0f, h, 0.0f, 0.0f, 1.0f, 0.0f  // Apex
        };

        // Connect the 12 generated vertices into the 4 faces of the pyramid
        std::vector<unsigned int> indices = {
            0, 1, 2, // Base
            3, 5, 4, // Back face
            6, 7, 8, // Left face
            9, 11, 10 // Right face
        };
        Upload(vertices, indices);
    }
};

// 6. The Torus (The Donut: a circle that circles another circle)
class Torus : public Shape3D
{
    public:
    Torus(float outerRadius = 0.5f, float innerRadius = 0.2f, int radialSegments = 32, int tubularSegments = 32)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float radialStep = 2 * PI / radialSegments;
        float tubularStep = 2 * PI / tubularSegments;

        // Uses a nested loop to wrap a tubular 'pipe' into a donut shape
        for (int i = 0; i <= radialSegments; ++i) {
            float radialAngle = i * radialStep;
            for (int j = 0; j <= tubularSegments; ++j) {
                float tubularAngle = j * tubularStep;

                // Vertex calculation based on outer and inner radii
                float xPos = (outerRadius + innerRadius * cos(tubularAngle)) * cos(radialAngle);
                float yPos = (outerRadius + innerRadius * cos(tubularAngle)) * sin(radialAngle);
                float zPos = innerRadius * sin(tubularAngle);

                // Normal calculation points directly from the tube's center
                float nx = cos(tubularAngle) * cos(radialAngle);
                float ny = cos(tubularAngle) * sin(radialAngle);
                float nz = sin(tubularAngle);

                vertices.push_back(xPos); vertices.push_back(yPos); vertices.push_back(zPos);
                vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            }
        }

        // Connect the grid of tube vertices into triangles
        for (int i = 0; i < radialSegments; ++i) {
            for (int j = 0; j < tubularSegments; ++j) {
                unsigned int first = (i * (tubularSegments + 1)) + j;
                unsigned int second = first + tubularSegments + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
        Upload(vertices, indices);
    }
};

// 7. The Plane (A flat, unit infinite-seeming floor or wall, 100x100 units)
class Plane : public Shape3D
{
    public:
    Plane()
    {
        r = 0.5f; g = 0.5f; b = 0.5f; // Gray floor
        x = 0; y = 0; z = 0; scale = 100.0f; // Massive scale
        drawMode = GL_TRIANGLES;

        float h = 0.5f;
        // Simple 4-vertex quad that lays flat on the X-Z ground plane
        std::vector<float> vertices = {
            -h, 0.0f, -h,  0.0f, 1.0f, 0.0f, // Vertex 0, Normal pointing Up (Y+)
             h, 0.0f, -h,  0.0f, 1.0f, 0.0f,
             h, 0.0f,  h,  0.0f, 1.0f, 0.0f,
            -h, 0.0f,  h,  0.0f, 1.0f, 0.0f,
        };

        // Create the 2 triangles needed for a floor quad
        std::vector<unsigned int> indices = {0, 3, 2, 0, 2, 1};
        Upload(vertices, indices);
    }
};

// 8. The Icosahedron (A 20-sided regular solid made of identical equilateral triangles)
class Icosahedron : public Shape3D
{
    public:
    Icosahedron(float radius = 1.0f)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = radius;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Use the golden ratio 'Phi' to calculate the coordinates for an Icosahedron
        float phi = (1.0f + sqrt(5.0f)) / 2.0f;
        float invLen = 1.0f / sqrt(phi * phi + 1.0f); // needed for normalized normals
        phi *= invLen; // normalize Phi

        std::vector<std::array<float, 3>> temp_vertices = {
            {-invLen, phi, 0.0f}, {invLen, phi, 0.0f}, {-invLen, -phi, 0.0f}, {invLen, -phi, 0.0f},
            {0.0f, -invLen, phi}, {0.0f, invLen, phi}, {0.0f, -invLen, -phi}, {0.0f, invLen, -phi},
            {phi, 0.0f, -invLen}, {phi, 0.0f, invLen}, {-phi, 0.0f, -invLen}, {-phi, 0.0f, invLen}
        };

        std::vector<std::array<unsigned int, 3>> temp_indices = {
            {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
            {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
            {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
            {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
        };

        // Loop through the 20 faces, creating separate vertices with flat normals for each face
        for (const auto& triangle : temp_indices) {
            std::array<float, 3> v0 = temp_vertices[triangle[0]];
            std::array<float, 3> v1 = temp_vertices[triangle[1]];
            std::array<float, 3> v2 = temp_vertices[triangle[2]];

            // Calculate the triangle's surface normal by finding the vector cross product (v1-v0) x (v2-v0)
            float nx = (v1[1] - v0[1]) * (v2[2] - v0[2]) - (v1[2] - v0[2]) * (v2[1] - v0[1]);
            float ny = (v1[2] - v0[2]) * (v2[0] - v0[0]) - (v1[0] - v0[0]) * (v2[2] - v0[2]);
            float nz = (v1[0] - v0[0]) * (v2[1] - v0[1]) - (v1[1] - v0[1]) * (v2[0] - v0[0]);
            float len = sqrt(nx*nx + ny*ny + nz*nz);
            if (len > 0) { nx /= len; ny /= len; nz /= len; } // Normalize normal

            unsigned int start = vertices.size() / 6;
            // Push v0 data (pos + normal)
            vertices.push_back(v0[0]); vertices.push_back(v0[1]); vertices.push_back(v0[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            // Push v1 data
            vertices.push_back(v1[0]); vertices.push_back(v1[1]); vertices.push_back(v1[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            // Push v2 data
            vertices.push_back(v2[0]); vertices.push_back(v2[1]); vertices.push_back(v2[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            
            // Link these new vertices into a triangle
            indices.push_back(start); indices.push_back(start+1); indices.push_back(start+2);
        }
        Upload(vertices, indices);
    }
};

// 9. The Prism (A 6-sided triangular prism)
class Prism : public Shape3D
{
    public:
    Prism(float height = 1.0f, float size = 1.0f)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = 1.0f;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float h_half = height / 2.0f;
        float h_tri = (sqrt(3.0f)/(2.0f) * size);
        float y_off = h_tri / 3.0f;

        // Bottom triangle coordinates
        float b0_x = 0.0f;      float b0_y = -h_half; float b0_z = 2.0f * y_off;
        float b1_x = -size/2.0f; float b1_y = -h_half; float b1_z = -y_off;
        float b2_x = size/2.0f; float b2_y = -h_half; float b2_z = -y_off;

        // Top triangle coordinates
        float t0_x = b0_x; float t0_y = h_half; float t0_z = b0_z;
        float t1_x = b1_x; float t1_y = h_half; float t1_z = b1_z;
        float t2_x = b2_x; float t2_y = h_half; float t2_z = b2_z;

        // Uses a lambda function helper to quickly push position + normal data
        auto push_vert = [&](float x, float y, float z, float nx, float ny, float nz) {
            vertices.push_back(x);  vertices.push_back(y);  vertices.push_back(z);
            vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);
        };

        // 1. Bottom and Top faces with pure flat normals (0, -1, 0) and (0, 1, 0)
        push_vert(b0_x, b0_y, b0_z, 0.0f, -1.0f, 0.0f); // B0 (0)
        push_vert(b1_x, b1_y, b1_z, 0.0f, -1.0f, 0.0f); // B1 (1)
        push_vert(b2_x, b2_y, b2_z, 0.0f, -1.0f, 0.0f); // B2 (2)
        indices.push_back(0); indices.push_back(2); indices.push_back(1);

        push_vert(t0_x, t0_y, t0_z, 0.0f, 1.0f, 0.0f); // T0 (3)
        push_vert(t1_x, t1_y, t1_z, 0.0f, 1.0f, 0.0f); // T1 (4)
        push_vert(t2_x, t2_y, t2_z, 0.0f, 1.0f, 0.0f); // T2 (5)
        indices.push_back(3); indices.push_back(4); indices.push_back(5);

        // 2. Side Quad: Connecting B1, B2, T2, T1 (Normal points forward in Z+)
        unsigned int s1 = 6;
        push_vert(b1_x, b1_y, b1_z, 0.0f, 0.0f, 1.0f); // B1_s1
        push_vert(b2_x, b2_y, b2_z, 0.0f, 0.0f, 1.0f); // B2_s1
        push_vert(t2_x, t2_y, t2_z, 0.0f, 0.0f, 1.0f); // T2_s1
        push_vert(t1_x, t1_y, t1_z, 0.0f, 0.0f, 1.0f); // T1_s1
        indices.push_back(s1); indices.push_back(s1+2); indices.push_back(s1+1); 
        indices.push_back(s1); indices.push_back(s1+3); indices.push_back(s1+2);

        // 3. Side Quad: Connecting B2, B0, T0, T2 (Normal approximated)
        unsigned int s2 = 10;
        float nx_s2 = cos(PI/6.0f); // nx = sqrt(3)/2
        push_vert(b2_x, b2_y, b2_z, nx_s2, 0.0f, -0.5f); // B2_s2
        push_vert(b0_x, b0_y, b0_z, nx_s2, 0.0f, -0.5f); // B0_s2
        push_vert(t0_x, t0_y, t0_z, nx_s2, 0.0f, -0.5f); // T0_s2
        push_vert(t2_x, t2_y, t2_z, nx_s2, 0.0f, -0.5f); // T2_s2
        indices.push_back(s2); indices.push_back(s2+2); indices.push_back(s2+1); 
        indices.push_back(s2); indices.push_back(s2+3); indices.push_back(s2+2);

        // 4. Side Quad: Connecting B0, B1, T1, T0
        unsigned int s3 = 14;
        push_vert(b0_x, b0_y, b0_z, -nx_s2, 0.0f, -0.5f); // B0_s3
        push_vert(b1_x, b1_y, b1_z, -nx_s2, 0.0f, -0.5f); // B1_s3
        push_vert(t1_x, t1_y, t1_z, -nx_s2, 0.0f, -0.5f); // T1_s3
        push_vert(t0_x, t0_y, t0_z, -nx_s2, 0.0f, -0.5f); // T0_s3
        indices.push_back(s3); indices.push_back(s3+2); indices.push_back(s3+1); 
        indices.push_back(s3); indices.push_back(s3+3); indices.push_back(s3+2);

        Upload(vertices, indices);
    }
};

// 10. The Dodecahedron (A beautiful 12-sided solid made of regular pentagons)
class Dodecahedron : public Shape3D
{
    public:
    Dodecahedron(float radius = 1.0f)
    {
        r = 1.0f; g = 1.0f; b = 1.0f;
        x = 0; y = 0; z = 0; scale = radius;
        drawMode = GL_TRIANGLES;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float phi = (1.0f + sqrt(5.0f)) / 2.0f;
        float h = 1.0f / radius;
        
        // Use coordinates derived from Golden Ratio 'phi' and its inverse 'h'
        std::vector<std::array<float, 3>> temp_vertices = {
            {h, h, h}, {h, h, -h}, {h, -h, h}, {h, -h, -h},
            {-h, h, h}, {-h, h, -h}, {-h, -h, h}, {-h, -h, -h},
            {0.0f, phi*h, h/phi}, {0.0f, phi*h, -h/phi}, {0.0f, -phi*h, h/phi}, {0.0f, -phi*h, -h/phi},
            {phi*h, h/phi, 0.0f}, {phi*h, -h/phi, 0.0f}, {-phi*h, h/phi, 0.0f}, {-phi*h, -h/phi, 0.0f},
            {h/phi, 0.0f, phi*h}, {h/phi, 0.0f, -phi*h}, {-h/phi, 0.0f, phi*h}, {-h/phi, 0.0f, -phi*h}
        };

        // Blueprint connecting the 20 vertices into 12 distinct pentagonal faces (defined by 5 vertices)
        std::vector<std::array<unsigned int, 5>> pentagon_faces = {
            {0, 8, 9, 1, 12}, {0, 12, 13, 2, 16}, {0, 16, 18, 4, 8}, {8, 4, 14, 5, 9}, {1, 9, 5, 19, 17}, 
            {1, 17, 3, 13, 12}, {13, 3, 11, 10, 2}, {16, 2, 10, 6, 18}, {18, 6, 15, 14, 4}, {19, 7, 11, 3, 17},
            {19, 5, 14, 15, 7}, {6, 10, 11, 7, 15}
        };

        // Process each of the 12 pentagonal faces
        for (const auto& face : pentagon_faces) {
            std::array<float, 3> v0 = temp_vertices[face[0]];
            std::array<float, 3> v1 = temp_vertices[face[1]];
            std::array<float, 3> v2 = temp_vertices[face[2]];
            std::array<float, 3> v3 = temp_vertices[face[3]];
            std::array<float, 3> v4 = temp_vertices[face[4]];

            // Compute face normal: ((v1-v0) x (v2-v0))
            float nx = (v1[1] - v0[1]) * (v2[2] - v0[2]) - (v1[2] - v0[2]) * (v2[1] - v0[1]);
            float ny = (v1[2] - v0[2]) * (v2[0] - v0[0]) - (v1[0] - v0[0]) * (v2[2] - v0[2]);
            float nz = (v1[0] - v0[0]) * (v2[1] - v0[1]) - (v1[1] - v0[1]) * (v2[0] - v0[0]);
            float len = sqrt(nx*nx + ny*ny + nz*nz);
            if (len > 0) { nx /= len; ny /= len; nz /= len; } // Normalize

            // Generate face center by averaging the coordinates
            float cx = (v0[0] + v1[0] + v2[0] + v3[0] + v4[0]) / 5.0f;
            float cy = (v0[1] + v1[1] + v2[1] + v3[1] + v4[1]) / 5.0f;
            float cz = (v0[2] + v1[2] + v2[2] + v3[2] + v4[2]) / 5.0f;

            unsigned int baseIdx = vertices.size() / 6;
            // Add the Center Point as the first vertex of the face-specific vertices
            vertices.push_back(cx); vertices.push_back(cy); vertices.push_back(cz);
            vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);

            // Add the 5 boundary vertices
            vertices.push_back(v0[0]); vertices.push_back(v0[1]); vertices.push_back(v0[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            vertices.push_back(v1[0]); vertices.push_back(v1[1]); vertices.push_back(v1[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            vertices.push_back(v2[0]); vertices.push_back(v2[1]); vertices.push_back(v2[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            vertices.push_back(v3[0]); vertices.push_back(v3[1]); vertices.push_back(v3[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);
            vertices.push_back(v4[0]); vertices.push_back(v4[1]); vertices.push_back(v4[2]);
            vertices.push_back(nx);   vertices.push_back(ny);   vertices.push_back(nz);

            // Connect the center to the 5 boundary vertices to form 5 triangles (fan style)
            indices.push_back(baseIdx); indices.push_back(baseIdx + 1); indices.push_back(baseIdx + 2);
            indices.push_back(baseIdx); indices.push_back(baseIdx + 2); indices.push_back(baseIdx + 3);
            indices.push_back(baseIdx); indices.push_back(baseIdx + 3); indices.push_back(baseIdx + 4);
            indices.push_back(baseIdx); indices.push_back(baseIdx + 4); indices.push_back(baseIdx + 5);
            indices.push_back(baseIdx); indices.push_back(baseIdx + 5); indices.push_back(baseIdx + 1);
        }
        Upload(vertices, indices);
    }
};

#endif


