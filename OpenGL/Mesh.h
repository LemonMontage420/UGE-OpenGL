#pragma once

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"TextureClass.h"
#include"Camera.h"

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

enum class Primitive
{
	Triangle, Quad, CircleMesh, Pyramid, Cube, UVSphere, IcoSphere, CubeSphere, Capsule, Cone, Cylinder, Torus
};

class Mesh 
{
	public:
		//Primitive Rendering
		Mesh(Primitive& primitiveType, unsigned int detailLevel);
		//Indexed Mesh (Render Via Indices)
		Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

		void Draw(Shader& shader, Camera& camera, Transform& transform);
		void Delete();

		std::vector <Vertex> vertices;
		std::vector <GLuint> indices;

	private:

		VAO VAO;

		bool renderViaIndices;
};