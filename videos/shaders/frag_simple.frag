#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 Color;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;

void main()
{
    // reflection cofficient any material abosrb some of the light
    // so i think all the main three lights should have the same RC
    float RC = 0.89f;

    /*
    there are 3 main caclulations per fragment
    1 - ambient lighting the light that if reflected from the surroundings and get reflected in all direction
    2 - diffuse lighting the light coming from the light source and get reflected in all dirctions(diffused)
    3 - specular lighting the light coming from the light source and get reflected abroxmatilly like a mirror
    */

    // ambient lighting
    float ambientIntensity = 0.1f;
    vec3 ambient_I = lightColor * 0.1f * RC;

    // diffuse lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    // cos(angle between light direction and surface normal) = lightDir . norm

    vec3 diffusion_I = RC * max(dot(lightDir, norm), 0.0f) * lightColor;

    // specular lighting

    // vector from fragment position to camera position
    vec3 toCameraVector = normalize(cameraPos - FragPos);
    vec3 reflectedRay = reflect(-lightDir, norm);

    vec3 specular_I = RC * pow(max(dot(reflectedRay, toCameraVector), 0.0f), 80) * lightColor;

    vec3 I = (ambient_I + diffusion_I + specular_I) * vec3(Color);

    FragColor = vec4(I, 1.0f);
    
    

    
}