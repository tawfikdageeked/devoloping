#ifndef OBJECTS_HPP
#define OBJECTS_HPP


#include <GL/glew.h>
#include <array>
#include <map>
#include <string>
#include <cmath>
#include "ShaderManager.hpp"


class Shape
{   
    public:

    unsigned int VAO;
    unsigned int VBO;
    int vertexcount;
    float r, g, b, opacity;
    float x, y, z;

    void SetColor(const float color[3], float alpha = 1.0f)
    {
        r = color[0];
        g = color[1];
        b = color[2];
        opacity = alpha;
    }

    void SetColor(float red, float green, float blue, float alpha = 1.0f)
    {
        r = red;
        g = green;
        b = blue;
        opacity = alpha;
    }


    void Upload(float* vertices, int totalfloats)
    {
        vertexcount = totalfloats / 3;
        glGenVertexArrays(1, &VAO);
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
        ShaderManager.UseProgram("DefaultShader");
        ShaderManager.SetVec3("DefaultShader", "uColor", r, g, b);
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

    Triangle(float size, float red =1.0f, float green= 0.0f, float blue = 0.0f)
    {
        sizeLength = size;

        r = red; g = green; b = blue;
        x = 0; y = 0; z = 0;

        float h = (sqrt(3.0f)/(2.0f) * size);

        float vertices[] = {
            0.0f, 2.0f * h / 3.0f, 0.0f,

            -size / 2, -h / 3.0f, 0.0f,

            size / 2, -h / 3.0f, 0.0f
        };

        Upload(vertices, 9);
    };
    
};


#endif