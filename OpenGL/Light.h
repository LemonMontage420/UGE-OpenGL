#pragma once
#include"Model.h"

enum class LightType
{
	Directional, Point, Spot
};

class Light 
{
	public:

		Light(LightType lightType);

		Transform transform;
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		float power = 1.0f;

		void DrawLight(Shader& lightShader, Camera& camera);
	private:
		std::vector<Vertex> lightsVertices;
		std::vector<GLuint> lightIndices;
};
