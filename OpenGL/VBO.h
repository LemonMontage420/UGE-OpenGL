#pragma once

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 texCoords;
	glm::vec4 color;
};

class VBO 
{
	public:
		GLuint ID;
		VBO(std::vector<Vertex>& vertices, GLenum drawType);

		void Bind();
		void Unbind();
		void Delete();
};