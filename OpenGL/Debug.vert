#version 330 core

//Layouts (Per-Vertex) (Should Find A Way To Keep From Having To Send Unnecessary Layout Info For Lights)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aCol;

//Uniforms (Per-Shader)
uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
	vec3 localPosition = aPos;
    vec3 worldPosition = vec3(modelMatrix * vec4(localPosition, 1.0));
    gl_Position = camMatrix * modelMatrix * vec4(localPosition, 1.0);
}