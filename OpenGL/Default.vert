#version 330 core

//Layouts (Per-Vertex)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aCol;

//Uniforms (Per-Shader)
uniform mat4 camMatrix;
uniform mat4 modelMatrix;

//Variables To Move Down The Shader Pipeline Other Than Predefined Ones Like gl_Position
out vec3 vertexNormals;
out vec2 textureCoordinates;
out vec4 vertexColor;

void main()
{
	vertexNormals = aNorm; //Pass The Vertex Color Info Onto The Frag Shader
	textureCoordinates = aTexCoord; //Pass The Texture Coordinates Of Each Vertex Onto The Frag Shader
	vertexColor = aCol; //Pass The Vertex Color Info Onto The Frag Shader

    gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);
}