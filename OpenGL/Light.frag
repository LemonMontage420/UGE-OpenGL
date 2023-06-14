#version 330 core

//Output Variables
out vec4 FragColor;

//Variables That Came From Earlier On In The Shader Pipeline (Vertex Shader In This Case)
in vec3 localPosition;
in vec3 worldPosition;

in vec3 norm;
in vec2 textureCoordinates;

//Uniforms (Per-Shader)
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec3 diffuse;

    vec3 normal = normalize(norm);
    vec3 lightDirection = normalize(lightPos - worldPosition);

    diffuse = max(dot(normal, lightDirection), 0.0f) * lightColor;

    vec3 texCol = texture(texture1, textureCoordinates).rgb;
    
    //Set The Fragment Color To The Light Color Uniform
    FragColor = vec4(texCol, 0.0f);
}