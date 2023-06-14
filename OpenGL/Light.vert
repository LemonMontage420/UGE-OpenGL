#version 330 core

//Layouts (Per-Vertex) (Should Find A Way To Keep From Having To Send Unnecessary Layout Info For Lights)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aCol;

//Variables To Move Down The Shader Pipeline Other Than Predefined Ones Like gl_Position
out vec3 localPosition;
out vec3 worldPosition;

out vec3 norm;
out vec2 textureCoordinates;

//Uniforms (Per-Shader)
uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
    norm = vec3(modelMatrix * vec4(aNorm, 1.0f));
    textureCoordinates = aTexCoord;

	localPosition = aPos;
    worldPosition = vec3(modelMatrix * vec4(localPosition, 1.0));
    gl_Position = camMatrix * modelMatrix * vec4(localPosition, 1.0);
}