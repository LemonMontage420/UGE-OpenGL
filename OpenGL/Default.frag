#version 330 core

//Output Variables
out vec4 FragColor;

//Variables That Came From Earlier On In The Shader Pipeline (Vertex Shader In This Case)
in vec3 vertexNormals;
in vec2 textureCoordinates;
in vec4 vertexColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec4 lightCol;

void main()
{
    //Sample The Imported Texture And Multipy By The Vertex Color
    FragColor = mix(texture(texture0, textureCoordinates), texture(texture1, textureCoordinates), 0.5f) * vertexColor;
}