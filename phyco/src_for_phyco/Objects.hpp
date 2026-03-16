#ifndef OBJECTS_HPP
#define OBJECTS_HPP


#include <GL/glew.h>
#include <array>
#include <map>
#include <string>
#include <cmath>


class Shape
{   
    public:

    unsigned int VAO;
    unsigned int VBO;
    int vertexcount;
    float r, g, b;
    float x, y, z;


    void Upload(float* vertices, int totalfloats)
    {
        vertexcount = totalfloats / 3;
        glGenVertexArrays(1, &VBO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, totalfloats * sizeof(float), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexcount);
    }

    void Delete()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    };

};

class Triangle : public Shape
{
    public:

    float sizeLength;

    Triangle(float size, float red, float green, float blue)
    {
        sizeLength = size;
        r = red;
        g = green;
        b = blue;
        x = 0; y = 0; z = 0;

        float h = (sqrt(3.0f)/(2.0f) * size);

        float vertices[] = {
            0.0f, 2.0f * h / 0.3f, 0.0f,

            -size / 2, -h / 3.0f, 0.0f,

            size / 2, -h / 3.0f, 0.0f
        };

        Upload(vertices, 9);
    };
    
};

#endif