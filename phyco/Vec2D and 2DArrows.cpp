#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>

struct Vec2D
{

    // components of vector
    float x;
    float y;

    // costructors
    Vec2D() : x(0), y(0) {}
    Vec2D(float x, float y) : x(x), y(y) {}

    // operators on vetctors

    // addintion of vecors VECTOR + VECTOR
    Vec2D operator+(const Vec2D& v) const
    {
        return Vec2D(x + v.x, y + v.y);
    }

    // substaraction of vector VECTOR - VECTOR
    Vec2D operator-(const Vec2D& v) const
    {
        return Vec2D(x - v.x, y - v.y);
    }

    // multiply by scaler VECTOR * S
    Vec2D operator*(float s) const
    {
        return Vec2D(s * x, s * y);
    }

    // Methods

    // get length of vector
    float GetLen() const
    {
        return (float) std::sqrt((x * x) + (y * y));
    }

    //get unit vector in direction of input vector
    Vec2D Normalize() const
    {
        
        float l = GetLen();
        if(l != 0) {return Vec2D(x / l, y / l);}
        else {std::cout << "Warning!!!\nCan't Divide By Zero. Revise Your Math WE DON'T WANT BLACK HOLES\n"; return Vec2D(0, 0);}
        
    }

    Vec2D Perp() const
    {
        return Vec2D(-y , x);
    }
};

struct Arrow
{
    public:
        
    // constructors
    Arrow(Vec2D s, Vec2D e) : start(s), end(e)
    {
        Update();
    }

    Arrow(std::array<float, 2> s, std::array<float, 2> e)
    : Arrow(Vec2D(s[0], s[1]), Vec2D(e[0], e[1])) {}

    // Methods

    //change end point only
    void ChangeEnd(Vec2D NewEnd)
    {
        end = NewEnd;
        Update();
    }
    void ChangeEnd(std::array<float, 2> NewEnd)
    {
        ChangeEnd(Vec2D(NewEnd[0], NewEnd[1]));
    }

    // change start point only
    void ChangeStart(Vec2D NewStart)
    {
        start = NewStart;
        Update();
    }
    void ChangeStart(std::array<float, 2> NewStart)
    {
        ChangeStart(Vec2D(NewStart[0], NewStart[1]));
    }

    // change start and end point
    void ChangeStartAndEnd(Vec2D NewStart, Vec2D NewEnd)
    {
        start = NewStart;
        end = NewEnd;
        Update();
    }
    void ChangeStartAndEnd(std::array<float, 2> NewStart, std::array<float, 2> NewEnd)
    {
        ChangeStartAndEnd(Vec2D(NewStart[0], NewStart[1]), Vec2D(NewEnd[0], NewEnd[1]));
    }

    // set how far from the camera if used in 3D circumstances
    void SetZPosition(float z) {position[2] = z; Update();}
    

    // Geters
    const Vec2D& GetStart() const      {return start;}
    const Vec2D& GetEnd() const        {return end;}
    const Vec2D& GetVector() const     {return vec;}
    const Vec2D& GetDirection() const  {return uvec;}
          float GetLen() const         {return l;}
    const std::vector<float>& GetVertices()   const  {return vertices;}
    const std::array<float, 3>& GetPosition() const  {return position;}
    const std::array<unsigned int, 9>& GetIndices() const { return indices;}
    
    
private:
    
    
    // components of arrow
    Vec2D start;
    Vec2D end;
        
    // directions of arrow
    Vec2D vec;
    Vec2D uvec;
    Vec2D puvec;

        
    // length of arrow
    float l;
        
    // array for position
    std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
        
    // the base of the triangle will equale the height i think it looks better that way
    float h;
        
    // vetices of arrow
    std::vector<float> vertices;

    inline static const std::array<unsigned int, 9> indices =
    {
        1, 0, 6,
        3, 2, 5,
        3, 5, 4,
    };

        
    void Update()
    {
       
        vec   =  end - start;
        uvec  =  vec.Normalize();
        puvec =  uvec.Perp();
        l     =  vec.GetLen();
        h     =  0.15f;
        
        position[0] = (end + (uvec * (-0.5f * l))).x;
        position[1] = (end + (uvec * (-0.5f * l))).y;
        
        // calculating the vertices
        Vec2D vertex1 = (end + (uvec * -h)) + (puvec * (- 0.5f * h));
        Vec2D vertex2 = (end + (uvec * -h)) + (puvec * (-0.25f * h));
        Vec2D vertex3 = (end + (uvec * -l)) + (puvec * (-0.25f * h));
        Vec2D vertex4 = (end + (uvec * -l)) + (puvec * ( 0.25f * h));
        Vec2D vertex5 = (end + (uvec * -h)) + (puvec * ( 0.25f * h));
        Vec2D vertex6 = (end + (uvec * -h)) + (puvec * (  0.5f * h));

        // creating the vertices buffer
        vertices =
        {
            end.x,     end.y,     position[2],
            vertex1.x, vertex1.y, position[2],
            vertex2.x, vertex2.y, position[2],
            vertex3.x, vertex3.y, position[2],
            vertex4.x, vertex4.y, position[2],
            vertex5.x, vertex5.y, position[2],
            vertex6.x, vertex6.y, position[2],
        };
    }
};

const char* vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 460 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Arrow Test", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 600, 600);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Arrow arrow(std::array<float, 2>{0.0f,0.0f}, std::array<float, 2>{0.2f,0.3f});

    unsigned int VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

   
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, arrow.GetVertices().size() * sizeof(float), arrow.GetVertices().data(), GL_DYNAMIC_DRAW);
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrow.GetIndices().size() * sizeof(unsigned int), arrow.GetIndices().data(), GL_STATIC_DRAW);    
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);

    float current_time = glfwGetTime();
    float frame_time;
    float dt;
    float theta = 0;
    float x = 0;
    float y = 0;
    float l = 0;
    float m = 0;

    std::vector<Arrow> arrows =
    {
        Arrow(Vec2D(0.0f, 0.0f), Vec2D(0.2f, 0.3f)),
        Arrow(Vec2D(0.0f, 0.0f), Vec2D(-0.2f, 0.3f)),
        Arrow(Vec2D(0.0f, 0.0f), Vec2D(0.3f, -0.1f))
    };

    while(!glfwWindowShouldClose(window))
    {
        // Clean the hidden canvas
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark space background
        glClear(GL_COLOR_BUFFER_BIT);

        frame_time = glfwGetTime();
        dt = frame_time - current_time;
        current_time = frame_time;
        
        theta += (M_PI/6 * dt);
        Vec2D position(0.25f * std::cos(theta + M_PI), 0.25f * std::sin(theta * 5));
        x += 0.0003f * std::sin(theta) * std::pow(std::cos(theta), 2);
        y += 0.001f * std::cos(theta);
        l += std::cos(theta / dt) * x;
        m += std::sin(theta / dt) * pow(y, 3);
        
        arrows[0].ChangeStartAndEnd(Vec2D(x * l, y * m), position);
        arrows[1].ChangeStartAndEnd(Vec2D(x * std::cos(theta * 3), m * std::sin(theta * 0.333f)), Vec2D(l + 0.2f, y + 0.3f));
        arrows[2].ChangeStartAndEnd(Vec2D(l * std::sin(theta * 3), y * std::cos(theta * 0.333f)), Vec2D(-y + 0.1f, m + 0.2f));

        glBindVertexArray(VAO);
        
        for (const Arrow& arrow : arrows)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER,
            arrow.GetVertices().size() * sizeof(float),
            arrow.GetVertices().data(),
            GL_DYNAMIC_DRAW);

            glDrawElements(GL_TRIANGLES,
            arrow.GetIndices().size(),
            GL_UNSIGNED_INT,
            0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }


    
    

    
    

    return 0;
}
