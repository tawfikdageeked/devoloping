#version 450 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec4 Col;
layout (location = 2) in vec3 Nor;
layout (location = 3) in vec2 Tex;

out vec4 Color;
out vec3 FragPos;
out vec3 Normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat3 normalMatrix;


void main()
{

    Color = Col;
    FragPos = vec3(model * vec4(Pos, 1.0f));
    Normal = normalMatrix * Nor;
    
    gl_Position = perspective * view * model * vec4(Pos, 1.0);
}


