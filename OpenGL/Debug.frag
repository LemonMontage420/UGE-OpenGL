#version 330 core

//Output Variables
out vec4 FragColor;

//Uniforms (Per-Shader)
uniform vec3 debugColor;

void main()
{
    FragColor = vec4(debugColor, 0.0f);
}