

/*

DISCLAIMER : EVERYTHING I SAY IN THIS COMMENTS IS BASED ON THE STATE OF UNDERSTANDING OF THE TOPIC WHILE WRITING IT MIGHT BE NOT COMPLETE OR WRONG SO IT'S NOT LIKE THE ABSLUTE REFRENCE IT CAN BE WRONG

THE SHADERS

A shaders is simply a small program or big it depends but it is a program that get compiled inside our GPU

What is this program and why it is important ?

This program is what the GPU used to draw literally all the data it takes

The shaders tells the GPU How to read the data you pass to it and tells it what it means the GPU is dmubly smart it just neep a push by the shader and it will continue

let't abuse the simplicity and say we have to we have 2 types of shaders

1- Vertex Shaders

tell the gpu how to read the vertices and what each value mean (NOT FULL DIFINITION)

2- Fragment Shader

tell the GPU how to color our vertices and shapes (NOT FULL DIFINITION)

how to make those shaders 

1st step WRITE THEM

they can be txt files or strings inside you code

2nd step give CREATING AND COMPILING

you can have multible shaders your code so it can be very long so carrying the whole shader isnot efficient and waste memory so

your create a value to refrence each shader in your code the type of this refrence (NOT LIKE A C++ REFRENCE (&) I MEAN LIKE REFRENCE HERE ONLY AS A WORD) this value type is an asigned int 

why this type ? Light not a memory waste but big enough to have as many shaders as you want - easy for debugging if it is 0 then somehting is wrong - hard for the GPU to get it wrong



unsigned int Compile(const char* source, GLenum type)
    {
        unsigned int shader = glCreateShader(type); // create empty ansigned int that you store the refrence for your shader
        glShaderSource(shader, 1, &source, NULL); // tell OpenGL shader is my value i want to refernce my source with and NULL means read my source to its end
        glCompileShader(shader); // tell OpenGL to compile the source code refrenced by the vlue

        // some checking for debuuging
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, NULL, log);
            std::cout << "Shader compile error: " << log << std::endl;
            return 0;
        }
        // return the value of refrencing
        return shader;

    }


After compiling you have two programs that will sort of interpert the same data so making them one program is more efficient so we have to do this



  //this method takes two values that refrence two compiled shaders and create one big program that do everything 
  
  unsigned int LinkShaders(unsigned int vertex_shader, unsigned int fragment_shader)
  {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
        // some checking for debugging
            int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetProgramInfoLog(program, 512, NULL, log);
            std::cout << "Shader link error: " << log << std::endl;
            return 0;
            }
            
        
        
            //after creating this big progam you can delete the compiled shader you created now you have one program with one refrencing value

            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            return program;
    }





*/

#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP









#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>






class ShaderMngr
{
    public:

    std::map<std::string, unsigned int> shaders;

    void SetMat4(std::string name, const char* uniform, glm::mat4 matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaders[name], uniform), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    
    // this method is not made for all users i think most of the will use the Create() methods
    unsigned int Compile(const char* source, GLenum type)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, NULL, log);
            std::cout << "Shader compile error: " << log << std::endl;
            return 0;
        }

        return shader;

    }


    // this too same thing not for all users
    unsigned int LinkShaders(unsigned int vertex_shader, unsigned int fragment_shader)
    {
        unsigned int program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

            int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetProgramInfoLog(program, 512, NULL, log);
            std::cout << "Shader link error: " << log << std::endl;
            return 0;
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    unsigned int CreateShaderFromString(std::string name, const char* vertex_shader, const char* fragment_shader)
    {
        unsigned int ver_shader = Compile(vertex_shader, GL_VERTEX_SHADER);
            if(ver_shader == 0) {return 0;}

        unsigned int frag_shader = Compile(fragment_shader, GL_FRAGMENT_SHADER);
            if(frag_shader == 0) {return 0;}

        unsigned int program = LinkShaders(ver_shader, frag_shader);
            if(program == 0) {return 0;}

        shaders[name] = program;

        return program;
    }

    void UseProgram(std::string name)
    {
        glUseProgram(shaders[name]);
    }


    void SetFloat(std::string name, const char* uniform, float value)
    {
        glUniform1f(glGetUniformLocation(shaders[name], uniform), value);
    }

    void SetInt(std::string name, const char* uniform, int value)
    {
        glUniform1i(glGetUniformLocation(shaders[name], uniform), value);
    }

    void SetVec2(std::string name, const char* uniform, float x, float y)
    {
        glUniform2f(glGetUniformLocation(shaders[name], uniform), x, y);
    }

    void SetVec3(std::string name, const char* uniform, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(shaders[name], uniform), x, y, z);
    }

    void SetVec4(std::string name, const char* uniform, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(shaders[name], uniform), x, y, z, w);
    }
   
    void DeleteShader(std::string name)
    {
        glDeleteProgram(shaders[name]);
        shaders.erase(name);
    }
   
    void DeleteAllShader()
    {
        for (auto& pair : shaders)
        {
            glDeleteProgram(pair.second);
        }
        shaders.clear();
    }


        unsigned int UseDefaultShader()
    {
       const char* vert = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "uniform mat4 uModel;\n" 
        "uniform mat4 uView;\n"
        "uniform mat4 uProjection;\n"

        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"

        "void main() {\n"
        "    // 1. Calculate the position in the 3D world\n"
        "    FragPos = vec3(uModel * vec4(aPos, 1.0));\n"
        
        "    // 2. THE LIGHTING FIX: Rotate the normals so shadows move with the object!\n"
        "    Normal = mat3(transpose(inverse(uModel))) * aNormal;\n" 
        
        "    gl_Position = uProjection * uView * vec4(FragPos, 1.0);\n"
        "}\n";

        const char* frag = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 FragPos;\n"
        "in vec3 Normal;\n"
        "uniform vec3 uColor;\n"
        "uniform vec3 uCameraPos;\n" 

        "void main() {\n"
        "   vec3 lightPos = vec3(2.0, 4.0, 3.0);\n"
        "   vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"

        "   float ambientStrength = 0.25;\n"
        "   vec3 ambient = ambientStrength * lightColor;\n"

        "   vec3 norm = normalize(Normal);\n"
        "   vec3 lightDir = normalize(lightPos - FragPos);\n"
        "   float diff = max(dot(norm, lightDir), 0.0);\n"
        "   vec3 diffuse = diff * lightColor;\n"

        "   float specularStrength = 0.5;\n"
        "   vec3 viewDir = normalize(uCameraPos - FragPos);\n"
        "   vec3 reflectDir = reflect(-lightDir, norm);\n"
        "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n" 
        "   vec3 specular = specularStrength * spec * lightColor;\n"

        "   vec3 result = (ambient + diffuse + specular) * uColor;\n"
        "   FragColor = vec4(result, 1.0);\n"
        "}\n";

        return CreateShaderFromString("DefaultShader", vert, frag);
    }


};

ShaderMngr ShaderManager;




#endif