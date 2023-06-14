#version 330 core

//Layouts (Per-Vertex)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aCol;

//Variables To Move Down The Shader Pipeline Other Than Predefined Ones Like gl_Position
out vec3 localPosition;
out vec3 worldPosition;

out vec3 vertexNormals;
out vec2 textureCoordinates;
out vec4 vertexColor;
out vec3 worldNormal;

//Uniforms (Per-Shader)
uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
	vertexNormals = aNorm; //Pass The Vertex Color Info Onto The Frag Shader
	textureCoordinates = aTexCoord; //Pass The Texture Coordinates Of Each Vertex Onto The Frag Shader
	vertexColor = aCol; //Pass The Vertex Color Info Onto The Frag Shader

	localPosition = aPos;
	//worldPosition = vec3(modelMatrix * vec4(localPosition, 1.0)); //Is this correct?
    vec4 worldPos4 = (modelMatrix * vec4(localPosition, 1.0));
	worldPosition = vec3(worldPos4) / worldPos4.w; //Or this? or is it the same?
	worldNormal = vec3(modelMatrix * vec4(vertexNormals, 1.0f));
    gl_Position = camMatrix * modelMatrix * vec4(localPosition, 1.0);
}